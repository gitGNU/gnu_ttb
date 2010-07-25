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
#include "ttb-ui.h"
#include "ttb-paths.h"

#define UI_FILE datadir "glade/prefs.ui"

#define UI_GTK_PREFS_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                       UI_TYPE_GTK_PREFS, UIGtkPrefsPrivate))

G_DEFINE_TYPE(UIGtkPrefs, ui_gtk_prefs, G_TYPE_OBJECT)

struct _UIGtkPrefsPrivate
{
	TTBBase *base;
	TTBBase *base_clone;
	TTBUI *ui;
	GtkWidget *window;
	GtkWidget *apply_button;
	GtkListStore *list;
};

enum {
	UI_GTK_PREFS_PROP_0,
	UI_GTK_PREFS_PROP_UI
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
	case UI_GTK_PREFS_PROP_UI:
		priv->ui = TTB_UI(g_value_get_object(value));
		priv->base = ttb_ui_get_base(priv->ui);
		priv->base_clone = ttb_base_clone(priv->base);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

TTBBase*
ui_gtk_prefs_get_base(UIGtkPrefs *self)
{
	g_return_if_fail(UI_IS_GTK_PREFS(self));

	return self->priv->base_clone;
}

static void
ui_gtk_prefs_init(UIGtkPrefs *self)
{
	self->priv = UI_GTK_PREFS_GET_PRIVATE(self);
	self->priv->window = NULL;
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
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ui_gtk_prefs_parent_class)->finalize(gobject);
}

static void
ui_gtk_prefs_setup_list(UIGtkPrefs *self)
{
	g_return_if_fail(UI_IS_GTK_PREFS(self));

	GtkTreeIter iter;
	TTBBase *base = self->priv->base;
	GtkListStore *list_store = self->priv->list;
	GSList *list = ttb_base_get_entries_list(base);

	while (list) {
		DesktopItem *item = list->data;
		gtk_list_store_append(list_store, &iter);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_NAME, item->name,
		                   COLUMN_EXEC, item->exec,
		                   COLUMN_ICON, item->icon,
		                   -1);
		list = g_slist_next(list);
	}
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
	GError     *error = NULL;

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
	priv->list = GTK_LIST_STORE(gtk_builder_get_object(builder,
	                                                   "apps_list"));
	ui_gtk_prefs_setup_list(self);

	gtk_builder_connect_signals(builder, self);

	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(GTK_WIDGET(priv->window));

	return;
}

static void
ui_gtk_prefs_set_cell_text(UIGtkPrefs *self, int column, gchar *path_string,
                           gchar *new_text)
{
	g_return_if_fail(UI_IS_GTK_PREFS(self));

	GtkListStore *list_store = self->priv->list;
	GtkTreePath  *path = gtk_tree_path_new_from_string(path_string);
	GtkTreeIter   iter;
	gtk_tree_model_get_iter(GTK_TREE_MODEL(list_store), &iter, path);
	gtk_list_store_set(list_store, &iter, column, new_text, -1);
}

G_MODULE_EXPORT void
cb_name_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text,
               gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;
	ui_gtk_prefs_set_cell_text(self, COLUMN_NAME, path_string, new_text);
	ttb_base_set_entry_name(priv->base_clone, atoi(path_string), new_text);
}

G_MODULE_EXPORT void
cb_exec_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text,
               gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;
	ui_gtk_prefs_set_cell_text(self, COLUMN_EXEC, path_string, new_text);
	ttb_base_set_entry_exec(priv->base_clone, atoi(path_string), new_text);
}

G_MODULE_EXPORT void
cb_icon_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text,
               gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;
	ui_gtk_prefs_set_cell_text(self, COLUMN_ICON, path_string, new_text);
	ttb_base_set_entry_icon(priv->base_clone, atoi(path_string), new_text);
}

G_MODULE_EXPORT void
cb_apply_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	GSList *list = ttb_base_get_entries_list(priv->base_clone);
	ttb_base_set_entries_list(priv->base, list);
	gtk_widget_set_sensitive(widget, FALSE);

	ttb_ui_rebuild(priv->ui);
}

G_MODULE_EXPORT void
cb_ok_clicked(GtkWidget *widget, gpointer data)
{
	UIGtkPrefs *self = UI_GTK_PREFS(data);
	UIGtkPrefsPrivate *priv = self->priv;

	cb_apply_clicked(priv->apply_button, data);
	gtk_widget_hide(priv->window);
}

G_MODULE_EXPORT void
cb_add_clicked(GtkWidget *widget, gpointer data)
{
}

G_MODULE_EXPORT void
cb_remove_clicked(GtkWidget *widget, gpointer data)
{
}

G_MODULE_EXPORT void
cb_new_clicked(GtkWidget *widget, gpointer data)
{
}

static void
ui_gtk_prefs_class_init(UIGtkPrefsClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	GParamSpec *pspec;
	
	gobject_class->dispose      = ui_gtk_prefs_dispose;
	gobject_class->finalize     = ui_gtk_prefs_finalize;
	gobject_class->set_property = ui_gtk_prefs_set_property;

	pspec = g_param_spec_object("UI", "UI", "UI", TTB_TYPE_UI,
                                    G_PARAM_CONSTRUCT_ONLY
                                    | G_PARAM_WRITABLE);
	g_object_class_install_property(gobject_class, UI_GTK_PREFS_PROP_UI,
	                                pspec);

	klass->show_prefs = ui_gtk_prefs_show_prefs;

	g_type_class_add_private(klass, sizeof(UIGtkPrefsPrivate));
}

