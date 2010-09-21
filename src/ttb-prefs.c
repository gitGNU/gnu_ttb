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

#include "ttb-prefs.h"

G_DEFINE_TYPE(TTBPrefs, ttb_prefs, G_TYPE_OBJECT);

static void
ttb_prefs_init(TTBPrefs *self)
{
}

static void
ttb_prefs_class_init(TTBPrefsClass *klass)
{
}

gpointer
ttb_prefs_get_widget(TTBPrefs *self)
{
	TTBPrefsClass *klass = TTB_PREFS_GET_CLASS(self);
	return klass->get_widget(self);
}

void
ttb_prefs_add_dir(TTBPrefs *self, const char *dirname)
{
	TTBPrefsClass *klass = TTB_PREFS_GET_CLASS(self);
	klass->add_dir(self, dirname);
}
