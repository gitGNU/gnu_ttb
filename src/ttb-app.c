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

#include "ttb-app.h"
#include "ttb-base.h"
#include "ttb-module.h"
#include "ttb-prefs.h"
#include "ui-gtk.h"

#define TTB_APP_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                  TTB_TYPE_APP, TTBAppPrivate))

G_DEFINE_TYPE(TTBApp, ttb_app, G_TYPE_OBJECT);

static TTBApp *app = NULL;

struct _TTBAppPrivate
{
	TTBUI *ui;
};

void
ttb_app_exec(TTBApp *self, int argc, char **argv)
{
	g_return_if_fail(TTB_IS_APP(self));
	TTBAppPrivate *priv = self->priv;

	gchar *dirname = g_build_filename(g_get_home_dir(),
	                                  ".local/share/applications/ttb",
	                                  NULL);
	TTBBase *base = g_object_new(TTB_TYPE_BASE, NULL);
	ttb_base_load_from_dir(base, dirname);
	g_free(dirname);

	priv->ui = g_object_new(UI_TYPE_GTK, "base", base, NULL);

	ttb_ui_exec(priv->ui, argc, argv);
}

TTBApp*
ttb_app_get()
{
	g_warn_if_fail(app != NULL); /* Shouldn't happen */
	return app;
}

gpointer
ttb_app_get_prefs(TTBApp *self)
{
	g_debug("[TTBApp::get_prefs]");
	TTBModule *module = ttb_module_new(MODULESDIR "/gtk-prefs.so");
	g_type_module_use(G_TYPE_MODULE(module));
	g_type_module_unuse(G_TYPE_MODULE(module));

	guint  n_prefs;
	GType *prefs = g_type_children(TTB_TYPE_PREFS, &n_prefs);

	return g_object_new(prefs[0], NULL);
}

static void
ttb_app_init(TTBApp *self)
{
	TTBAppPrivate *priv;
	self->priv = priv = TTB_APP_GET_PRIVATE(self);

	priv->ui = NULL;

	g_warn_if_fail(app == NULL); /* Shouldn't happen */
	app = self;
}

static void
ttb_app_dispose(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_app_parent_class)->dispose(gobject);
}

static void
ttb_app_finalize(GObject *gobject)
{
	TTBApp *self = TTB_APP(gobject);
	g_object_unref(self->priv->ui);

	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_app_parent_class)->finalize(gobject);
}

static void
ttb_app_class_init(TTBAppClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	
	gobject_class->dispose  = ttb_app_dispose;
	gobject_class->finalize = ttb_app_finalize;

	g_type_class_add_private(klass, sizeof(TTBAppPrivate));
}

