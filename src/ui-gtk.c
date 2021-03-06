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

#include <libintl.h>
#define _(String) gettext(String)

#include "ui-gtk.h"
#include "ttb-app.h"
#include "gtk-prefs-module.h"
#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include "ttb-paths.h"

#define UI_GTK_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                 UI_TYPE_GTK, UIGtkPrivate))

G_DEFINE_TYPE(UIGtk, ui_gtk, TTB_TYPE_UI)

struct _UIGtkPrivate
{
	GtkWidget *window;
	TTBPrefs *prefs;
};

static void
ui_gtk_init(UIGtk *self)
{
	self->priv = UI_GTK_GET_PRIVATE(self);
	UIGtkPrivate *priv = self->priv;

	priv->window = NULL;
	priv->prefs  = NULL;
}

static void
ui_gtk_dispose(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ui_gtk_parent_class)->dispose(gobject);
}

static void
ui_gtk_finalize(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ui_gtk_parent_class)->finalize(gobject);
}

static void
cb_button_clicked(GtkWidget *widget, gpointer data)
{
	ttb_base_execute((gchar *) data);
	gtk_main_quit();
}

static void
cb_prefs_button_clicked(GtkWidget *widget, gpointer data)
{
	UIGtk        *self = UI_GTK(data);
	UIGtkPrivate *priv = self->priv;
	GtkWindow    *prefs_window;

	/* TODO: This should go to TTBUI */
	if (!priv->prefs) {
		priv->prefs = TTB_PREFS(ttb_app_get_prefs(ttb_app_get()));
		ttb_prefs_connect_ui(priv->prefs, TTB_UI(self));
		gchar *dirname = g_build_filename(g_get_home_dir(),
	                                      ".local/share/applications/ttb",
	                                          NULL);
		ttb_prefs_add_dir(priv->prefs, dirname);
		g_free(dirname);
	}
	prefs_window = GTK_WINDOW(ttb_prefs_get_widget(priv->prefs));
	gtk_window_set_transient_for(prefs_window, GTK_WINDOW(priv->window));
	gtk_widget_show(GTK_WIDGET(prefs_window));
}

static void
ui_gtk_rebuild(TTBUI *self)
{
	g_return_if_fail(UI_IS_GTK(self));

	UIGtkPrivate *priv = UI_GTK(self)->priv;
	TTBUIClass *klass = TTB_UI_CLASS(ui_gtk_parent_class);

	GtkWidget *button, *sep, *hbox, *tbox;
	TTBBase *base;
	GSList  *list;

	klass->rebuild(self);

	if (priv->window) {
		GtkWidget *child = gtk_bin_get_child(GTK_BIN(priv->window));
		gtk_widget_hide(priv->window);
		gtk_widget_destroy(child);
	} else {
		priv->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_keep_above(GTK_WINDOW(priv->window), TRUE);
		gtk_window_set_position(GTK_WINDOW(priv->window),
		                        GTK_WIN_POS_CENTER);
		gtk_window_set_resizable(GTK_WINDOW(priv->window), FALSE);
		gtk_window_set_title(GTK_WINDOW(priv->window), "TTB");
	}

	hbox = gtk_hbox_new(FALSE, 1);
	tbox = gtk_hbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(priv->window), hbox);

	button = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(button),
	                     gtk_image_new_from_stock(GTK_STOCK_CLOSE,
	                                              GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(button, _("Close"));
	g_signal_connect(button, "clicked",
	                 G_CALLBACK(gtk_main_quit), self);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 0);
	sep = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX(hbox), sep, TRUE, FALSE, 0);

	base = ttb_ui_get_base(TTB_UI(self));
	list = TTB_BASE_GET_CLASS(base)->get_entries_list(base);
	sep = (list != NULL)? gtk_vseparator_new() : NULL;

	while (list) {
		GtkWidget   *ic;
		DesktopItem *item = list->data;
		button = gtk_button_new_with_label(item->name);
		if ((item->icon) && (strlen(item->icon) != 0)) {
			ic = gtk_image_new_from_icon_name(item->icon,
		                                 GTK_ICON_SIZE_LARGE_TOOLBAR);
		} else {
			ic = gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE,
			                         GTK_ICON_SIZE_LARGE_TOOLBAR);
		}
		gtk_button_set_image(GTK_BUTTON(button), ic);
		gtk_widget_set_tooltip_text(button, item->name);
		gtk_box_pack_start(GTK_BOX(tbox), button, TRUE, TRUE, 0);
		g_signal_connect(button, "clicked",
		                 G_CALLBACK(cb_button_clicked),
		                 item->exec);
		list = g_slist_next(list);
	}
	gtk_box_pack_start(GTK_BOX(hbox), tbox, TRUE, FALSE, 0);
	if (sep)
		gtk_box_pack_start(GTK_BOX(hbox), sep, TRUE, FALSE, 0);
	button = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(button),
	                     gtk_image_new_from_stock(GTK_STOCK_PREFERENCES,
	                                              GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(button, _("Preferences"));
	g_signal_connect(button, "clicked",
	                 G_CALLBACK(cb_prefs_button_clicked), self);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, FALSE, 0);

	gtk_widget_show_all(priv->window);
	g_signal_connect_swapped(G_OBJECT(priv->window), "destroy",
	                         G_CALLBACK(gtk_main_quit), NULL);
}

static gboolean
ui_gtk_exec(TTBUI *self, int argc, char **argv)
{
	g_return_val_if_fail(UI_IS_GTK(self), FALSE);

	gtk_init(&argc, &argv);

	ui_gtk_rebuild(self);

	gtk_main();

	return TRUE;
}

static void
ui_gtk_class_init(UIGtkClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	TTBUIClass   *ui_class      = TTB_UI_CLASS(klass);
	
	gobject_class->dispose  = ui_gtk_dispose;
	gobject_class->finalize = ui_gtk_finalize;

	ui_class->exec    = ui_gtk_exec;
	ui_class->rebuild = ui_gtk_rebuild;

	g_type_class_add_private(klass, sizeof(UIGtkPrivate));
}

