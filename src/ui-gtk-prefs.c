/*
 *   TabletToolbox, a toolbox application fot tablet PCs
 *   Copyright (C) 2010, Mariusz Adamski <mariusz.adamski@gmail.com>
 *
 *   This file is part of TabletToolbox.
 *
 *   TabletToolbox is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 * 
 *   TabletToolbox is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with TabletToolbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui-gtk-prefs.h"
#include <gtk/gtk.h>
#include <unistd.h>
#include "ttb-fbase.h"
#include "ttb-paths.h"

#define UI_FILE datadir "glade/prefs.ui"
#define APP_DIR "/usr/share/applications"

#define UI_GTK_PREFS_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                       UI_TYPE_GTK_PREFS, UIGtkPrefsPrivate))

G_DEFINE_TYPE(UIGtkPrefs, ui_gtk_prefs, G_TYPE_OBJECT)

struct _UIGtkPrefsPrivate
{
	TTBFBase    *base;
	TTBBase     *sysapp_db;
	GtkWidget   *window;
	GtkWidget   *apply_button;
	GtkTreeView *tree;
	GtkWidget   *app_chooser;
	GtkTreeView *sysapp_tree;
	GtkWidget   *icon_chooser;
	GtkIconView *icon_view;
	GList       *icon_list;
	int          pid_of_ttb;
};

enum {
	UI_GTK_PREFS_PROP_0,
	UI_GTK_PREFS_PROP_BASE
};

enum {
	COLUMN_NAME,
	COLUMN_EXEC,
	COLUMN_ICON
};

static void
ui_gtk_prefs_set_property(GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
	UIGtkPrefs *self = UI_GTK_PREFS(object);
	UIGtkPrefsPrivate *priv = self->priv;

	switch (property_id) {
	case UI_GTK_PREFS_PROP_BASE:
		priv->base = TTB_FBASE(g_value_get_object(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void
ui_gtk_prefs_init(UIGtkPrefs *self)
{
	self->priv = UI_GTK_PREFS_GET_PRIVATE(self);
	UIGtkPrefsPrivate *priv = self->priv;

	priv->sysapp_db   = NULL;
	priv->window      = NULL;
	priv->app_chooser = NULL;
	priv->icon_list   = NULL;
	priv->pid_of_ttb  = 0;
}

static void
ui_gtk_prefs_dispose(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ui_gtk_prefs_parent_class)->dispose(gobject);
}

static void
ui_gtk_prefs_finalize(GObject *gobject)
{
	UIGtkPrefs *self = UI_GTK_PREFS(gobject);
	UIGtkPrefsPrivate *priv = self->priv;

	if (priv->sysapp_db)
		g_object_unref(self->priv->sysapp_db);
	if (priv->icon_list) {
		GList *list = priv->icon_list;
		while (list) {
			g_free(list->data);
			list = g_list_next(list);
		}
		g_list_free(priv->icon_list);
	}

	/* Chain up to the parent class */
	G_OBJECT_CLASS(ui_gtk_prefs_parent_class)->finalize(gobject);
}

static void
setup_tree(TTBBase *base, GtkTreeView *tree)
{
	GtkTreeIter iter;
	GSList *list = ttb_base_get_entries_list(base);
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));

	while (list) {
		DesktopItem *item = list->data;
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
		                   COLUMN_NAME, item->name,
		                   COLUMN_EXEC, item->exec,
		                   COLUMN_ICON, item->icon,
		                   -1);
		list = g_slist_next(list);
	}
}

static void
setup_sysapp_tree(TTBBase *base, GtkTreeView *tree)
{
	GtkTreeIter iter;
	GSList *list = ttb_base_get_entries_list(base);
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));

	while (list) {
		DesktopItem *item = list->data;
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
		                   0, item->icon,
		                   1, item->name,
		                   -1);
		list = g_slist_next(list);
	}
}

static void
setup_icon_view(UIGtkPrefs *self, GtkIconView *view)
{
	UIGtkPrefsPrivate *priv = self->priv;

	GtkTreeIter iter;
	GtkListStore *store = GTK_LIST_STORE(gtk_icon_view_get_model(view));
	GtkIconTheme *theme = gtk_icon_theme_get_default();
	GList *list = gtk_icon_theme_list_icons(theme, "Applications");

	while (gtk_events_pending())
		gtk_main_iteration();

	if (priv->icon_list) {
		GList *l = priv->icon_list;
		while (l) {
			g_free(l->data);
			l = g_list_next(l);
		}
		g_list_free(priv->icon_list);
	}

	priv->icon_list = list;
	while (list) {
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 0, list->data, -1);
		list = g_list_next(list);
	}

	while (gtk_events_pending())
		gtk_main_iteration();
}

void
ui_gtk_prefs_show_prefs(UIGtkPrefs *self)
{
	g_return_if_fail(UI_IS_GTK_PREFS(self));

	UIGtkPrefsPrivate *priv = self->priv;
	if (priv->window) {
		gtk_widget_show(GTK_WIDGET(priv->window));
		return;
	}

	GtkBuilder *builder;
	GError *error = NULL;

	builder = gtk_builder_new();
	if(!gtk_builder_add_from_file(builder, UI_FILE, &error)) {
		g_warning("Error: %s", error->message);
		g_free(error);
		return;
	}
	priv->window = GTK_WIDGET(gtk_builder_get_object(builder,
	                                                 "prefs_window"));
	priv->apply_button = GTK_WIDGET(gtk_builder_get_object(builder,
	                                                      "apply_button"));
	priv->tree = GTK_TREE_VIEW(gtk_builder_get_object(builder,
	                                                  "apps_treeview"));
	priv->app_chooser = GTK_WIDGET(gtk_builder_get_object(builder,
	                                                      "app_chooser"));
	priv->sysapp_tree = GTK_TREE_VIEW(gtk_builder_get_object(builder,
	                                                       "sysapp_tree"));
	priv->icon_chooser = GTK_WIDGET(gtk_builder_get_object(builder,
	                                                      "icon_chooser"));
	priv->icon_view = GTK_ICON_VIEW(gtk_builder_get_object(builder,
	                                                       "icon_view"));

	setup_tree(TTB_BASE(priv->base), priv->tree);

	gtk_builder_connect_signals(builder, self);

	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(GTK_WIDGET(priv->window));
}

void
ui_gtk_prefs_set_pid_of_ttb(UIGtkPrefs *self, int pid)
{
	g_return_if_fail(UI_IS_GTK_PREFS(self));

	self->priv->pid_of_ttb = pid;
}

void
ui_gtk_prefs_set_busy_cursor(GtkWidget *widget, gboolean busy)
{
	GdkDisplay *display;
	GdkCursor  *cursor;

	if (!gtk_widget_get_realized(widget))
		return;

	display = gtk_widget_get_display(widget);

	if (busy)
		cursor = gdk_cursor_new_for_display(display, GDK_WATCH);
	else
		cursor = NULL;

	gdk_window_set_cursor(gtk_widget_get_window(widget), cursor);
	gdk_display_flush(display);

	if (cursor)
		gdk_cursor_unref(cursor);
}

static void
ui_gtk_prefs_set_cell_text(UIGtkPrefs *self, int column, gchar *path_string,
                           gchar *new_text)
{
	g_return_if_fail(UI_IS_GTK_PREFS(self));

	GtkTreeView *tree = self->priv->tree;
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	GtkTreePath *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;
	gtk_tree_model_get_iter(model, &iter, path);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, new_text, -1);
}

static void
restart_ttb(UIGtkPrefs *self)
{
	UIGtkPrefsPrivate *priv = self->priv;

	if (priv->pid_of_ttb == 0)
		return;
	
	char *ttb = bindir "ttb";
	GPid new_pid;

	/* Not portable */
	kill(priv->pid_of_ttb, SIGTERM);

	g_spawn_async(NULL, &ttb, NULL, 0, NULL, NULL, &new_pid, NULL);
	priv->pid_of_ttb = new_pid;
}

G_MODULE_EXPORT void
cb_name_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text,
               gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;
	ui_gtk_prefs_set_cell_text(self, COLUMN_NAME, path_string, new_text);
	ttb_fbase_set_entry_name(priv->base, atoi(path_string), new_text);
}

G_MODULE_EXPORT void
cb_exec_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text,
               gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;
	ui_gtk_prefs_set_cell_text(self, COLUMN_EXEC, path_string, new_text);
	ttb_fbase_set_entry_exec(priv->base, atoi(path_string), new_text);
}

G_MODULE_EXPORT void
cb_icon_editing_started(GtkCellRenderer *cell,
                        GtkCellEditable *editable,
                        const gchar     *path,
                        gpointer         data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	gtk_widget_show_now(priv->icon_chooser);

	if (!priv->icon_list) {
		ui_gtk_prefs_set_busy_cursor(GTK_WIDGET(priv->icon_chooser), TRUE);
		setup_icon_view(self, priv->icon_view);
		ui_gtk_prefs_set_busy_cursor(GTK_WIDGET(priv->icon_chooser), FALSE);
	}

}

G_MODULE_EXPORT void
cb_icon_chosen(GtkWidget *widget, gpointer *data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	GtkTreeIter iter;
	GtkIconView *view = priv->icon_view;
	GtkTreeModel *model = gtk_tree_view_get_model(priv->tree);
	GtkListStore *apps = GTK_LIST_STORE(model);
	GtkTreePath *path;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(priv->tree);
	TTBFBase *base = priv->base;

	gtk_widget_hide(priv->icon_chooser);

	if (!gtk_icon_view_get_cursor(view, &path, NULL))
		return;

	if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
		gint i = gtk_tree_path_get_indices(path)[0];
		GList *list = g_list_nth(priv->icon_list, i);
		gtk_tree_path_free(path);

		path = gtk_tree_model_get_path(model, &iter);
		i = gtk_tree_path_get_indices(path)[0];
		ttb_fbase_set_entry_icon(base, i, list->data);

		gtk_list_store_set(apps, &iter,
		                   COLUMN_ICON, list->data,
		                   -1);
		gtk_tree_path_free(path);
	}
}

G_MODULE_EXPORT void
cb_icon_clear(GtkWidget *widget, gpointer *data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	GtkTreeIter iter;
	GtkIconView *view = priv->icon_view;
	GtkTreeModel *model = gtk_tree_view_get_model(priv->tree);
	GtkListStore *apps = GTK_LIST_STORE(model);
	GtkTreeSelection *selection = gtk_tree_view_get_selection(priv->tree);
	TTBFBase *base = priv->base;

	gtk_widget_hide(priv->icon_chooser);

	if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
		gint i;
		GtkTreePath *path;

		path = gtk_tree_model_get_path(model, &iter);
		i = gtk_tree_path_get_indices(path)[0];
		ttb_fbase_set_entry_icon(base, i, "");

		gtk_list_store_set(apps, &iter,
		                   COLUMN_ICON, "",
		                   -1);
		gtk_tree_path_free(path);
	}
}

G_MODULE_EXPORT void
cb_apply_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	TTBFBase *fbase = TTB_FBASE(priv->base);
	ttb_fbase_save(fbase);
	gtk_widget_set_sensitive(widget, FALSE);

	restart_ttb(self);
}

G_MODULE_EXPORT void
cb_ok_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	cb_apply_clicked(priv->apply_button, data);
	gtk_main_quit();
}

G_MODULE_EXPORT void
cb_add_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	if (priv->sysapp_db == NULL) {
		ui_gtk_prefs_set_busy_cursor(GTK_WIDGET(priv->window), TRUE);
		priv->sysapp_db = g_object_new(TTB_TYPE_BASE, NULL);
		ttb_base_load_from_dir(priv->sysapp_db, APP_DIR);
		setup_sysapp_tree(priv->sysapp_db, priv->sysapp_tree);
		ui_gtk_prefs_set_busy_cursor(GTK_WIDGET(priv->window), FALSE);
	}

	gtk_widget_show(priv->app_chooser);
}

G_MODULE_EXPORT void
cb_remove_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	GtkTreeIter iter;
	GtkTreeView *tree = priv->tree; 
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	GtkTreeSelection *selection = gtk_tree_view_get_selection(tree);
	TTBFBase *base = priv->base;

	if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
		gint i;
		GtkTreePath *path;

		path = gtk_tree_model_get_path(model, &iter);
		i = gtk_tree_path_get_indices(path)[0];
 		gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

		ttb_fbase_remove_entry(base, i);

		gtk_tree_path_free(path);
	}
}

G_MODULE_EXPORT void
cb_new_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	GtkTreeIter iter;
	GtkTreeView *tree = priv->tree; 
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
	TTBFBase *base = priv->base;
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
	                   COLUMN_NAME, "",
	                   COLUMN_EXEC, "",
	                   COLUMN_ICON, "",
	                   -1);
	ttb_fbase_add_entry(base, "", "", "");
}

G_MODULE_EXPORT void
cb_sysapp_chosen(GtkWidget *widget, gpointer *data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	GtkTreeIter iter;
	GtkTreeView *tree = priv->sysapp_tree;
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	GtkListStore *apps = GTK_LIST_STORE(gtk_tree_view_get_model(
	                                    priv->tree));
	GtkTreeSelection *selection = gtk_tree_view_get_selection(tree);
	TTBFBase *base = priv->base;

	gtk_widget_hide(priv->app_chooser);

	if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
		gint i;
		GtkTreePath *path;
		DesktopItem *item;

		path = gtk_tree_model_get_path(model, &iter);
		i = gtk_tree_path_get_indices(path)[0];
		item = ttb_base_get_entry(priv->sysapp_db, i);
		ttb_fbase_add_entry(base, item->name, item->exec, item->icon);

		gtk_list_store_append(apps, &iter);
		gtk_list_store_set(apps, &iter,
		                   COLUMN_NAME, item->name,
		                   COLUMN_EXEC, item->exec,
		                   COLUMN_ICON, item->icon,
		                   -1);

		gtk_tree_path_free(path);
	}
}

static void
ui_gtk_prefs_class_init(UIGtkPrefsClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	GParamSpec *pspec;
	
	gobject_class->dispose      = ui_gtk_prefs_dispose;
	gobject_class->finalize     = ui_gtk_prefs_finalize;
	gobject_class->set_property = ui_gtk_prefs_set_property;

	pspec = g_param_spec_object("base", "base", "base", TTB_TYPE_FBASE,
	                            G_PARAM_CONSTRUCT_ONLY
	                            | G_PARAM_WRITABLE);
	g_object_class_install_property(gobject_class, UI_GTK_PREFS_PROP_BASE,
	                                pspec);

	klass->show_prefs     = ui_gtk_prefs_show_prefs;
	klass->set_pid_of_ttb = ui_gtk_prefs_set_pid_of_ttb;

	g_type_class_add_private(klass, sizeof(UIGtkPrefsPrivate));
}

