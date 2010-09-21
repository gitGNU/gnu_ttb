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

#include <config.h>

#include <gmodule.h>

#include <ttb-module.h>

G_DEFINE_TYPE(TTBModule, ttb_module, G_TYPE_TYPE_MODULE);

enum {
	TTB_MODULE_PROP_0,
	TTB_MODULE_PROP_FILENAME
};

static void
ttb_module_get_property(GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
	TTBModule *self = TTB_MODULE(object);
	
	switch (property_id) {
	case TTB_MODULE_PROP_FILENAME:
		g_value_set_string(value, self->filename);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void
ttb_module_set_property(GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
	TTBModule *self = TTB_MODULE(object);

	switch (property_id) {
	case TTB_MODULE_PROP_FILENAME:
		g_free(self->filename);
		self->filename = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void
ttb_module_init(TTBModule *self)
{
	self->filename = NULL;
	self->library  = NULL;
	self->load     = NULL;
	self->unload   = NULL;
}

static void
ttb_module_finalize(GObject *gobject)
{
	TTBModule *self = TTB_MODULE(gobject);

	g_free(self->filename);

	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_module_parent_class)->finalize(gobject);
}

static gboolean
module_load(GTypeModule *gmodule)
{
	TTBModule *module = TTB_MODULE(gmodule);

	g_return_val_if_fail(module->filename, FALSE);

	module->library = g_module_open(module->filename, 0);
	if (!module->library) {
		g_printerr("%s\n", g_module_error());
		return FALSE;
	}

	if (!g_module_symbol(module->library, "ttb_module_load",
	                     (gpointer*) &module->load) ||
	    !g_module_symbol(module->library, "ttb_module_unload",
	                     (gpointer*) &module->unload))
	{
		g_printerr("%s\n", g_module_error());
		g_module_close(module->library);
		return FALSE;
	}

	module->load(module);
	return TRUE;
}

static void
module_unload(GTypeModule *gmodule)
{
	TTBModule *module = TTB_MODULE(gmodule);

	module->unload(module);

	g_module_close(module->library);
	module->library = NULL;

	module->load   = NULL;
	module->unload = NULL;
}

TTBModule*
ttb_module_new(const gchar *filename)
{
	TTBModule *module;
	
	g_return_val_if_fail(filename != NULL, NULL);

	module = g_object_new(TTB_TYPE_MODULE, "filename", filename, NULL);

	return module;
}

static void
ttb_module_class_init(TTBModuleClass *klass)
{
	GObjectClass     *gobject_class     = G_OBJECT_CLASS(klass);
	GTypeModuleClass *type_module_class = G_TYPE_MODULE_CLASS(klass);
	GParamSpec *pspec;
	
	gobject_class->finalize     = ttb_module_finalize;
	gobject_class->get_property = ttb_module_get_property;
	gobject_class->set_property = ttb_module_set_property;

	type_module_class->load     = module_load;
	type_module_class->unload   = module_unload;

	pspec = g_param_spec_string("filename", "Filename",
	                            "Module filename", NULL,
                                    G_PARAM_CONSTRUCT_ONLY
                                    | G_PARAM_READWRITE);
	g_object_class_install_property(gobject_class,
	                                TTB_MODULE_PROP_FILENAME, pspec);
}

