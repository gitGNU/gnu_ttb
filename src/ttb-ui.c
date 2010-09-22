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

#include "ttb-ui.h"

#define TTB_UI_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                TTB_TYPE_UI, TTBUIPrivate))

G_DEFINE_TYPE(TTBUI, ttb_ui, G_TYPE_OBJECT);

struct _TTBUIPrivate
{
	TTBBase *base;
	gchar   *dirname;
};

enum {
	TTB_UI_PROP_0,
	TTB_UI_PROP_DIRNAME
};

static void
ttb_ui_set_property(GObject      *object,
                    guint         property_id,
                    const GValue *value,
                    GParamSpec   *pspec)
{
	TTBUI *self = TTB_UI(object);

	switch (property_id) {
	case TTB_UI_PROP_DIRNAME:
		self->priv->dirname = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void
ttb_ui_init(TTBUI *self)
{
	self->priv = TTB_UI_GET_PRIVATE(self);
	self->priv->base = NULL;
}

static void
ttb_ui_dispose(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_ui_parent_class)->dispose(gobject);
}

static void
ttb_ui_finalize(GObject *gobject)
{
	TTBUI *self = TTB_UI(gobject);
	g_object_unref(self->priv->base);

	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_ui_parent_class)->finalize(gobject);
}

TTBBase*
ttb_ui_get_base(TTBUI *self)
{
	g_return_val_if_fail(TTB_IS_UI(self), NULL);

	return self->priv->base;
}

void
ttb_ui_exec(TTBUI *self, int argc, char **argv)
{
	g_return_if_fail(TTB_IS_UI(self));

	TTBUIClass *klass = TTB_UI_GET_CLASS(self);
	g_return_if_fail(klass->exec);
	klass->exec(self, argc, argv);
}

void ttb_ui_rebuild(TTBUI *self)
{
	g_return_if_fail(TTB_IS_UI(self));

	TTBUIClass *klass = TTB_UI_GET_CLASS(self);
	g_return_if_fail(klass->rebuild);
	klass->rebuild(self);
}

static void
rebuild(TTBUI *self)
{
	g_return_if_fail(TTB_IS_UI(self));
	TTBUIPrivate *priv = self->priv;

	if (priv->base)
		g_object_unref(priv->base);

	priv->base = g_object_new(TTB_TYPE_BASE, NULL);
	ttb_base_load_from_dir(priv->base, priv->dirname);
}

static void
ttb_ui_class_init(TTBUIClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	GParamSpec *pspec;
	
	gobject_class->dispose      = ttb_ui_dispose;
	gobject_class->finalize     = ttb_ui_finalize;
	gobject_class->set_property = ttb_ui_set_property;

	pspec = g_param_spec_string("dirname", "dirname", "Apps DB dirname",
	                            "", G_PARAM_CONSTRUCT_ONLY
	                              | G_PARAM_WRITABLE);
	g_object_class_install_property(gobject_class, TTB_UI_PROP_DIRNAME,
	                                pspec);

	klass->exec = NULL;
	klass->rebuild = rebuild;

	g_type_class_add_private(klass, sizeof(TTBUIPrivate));
}

