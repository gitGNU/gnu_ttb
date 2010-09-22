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

#include <stdio.h>
#include <libintl.h>
#include "ttb-app.h"
#include "ttb-base.h"
#include "ui-gtk.h"
#include "ttb-paths.h"

int main(int argc, char **argv)
{
	TTBApp *app;

	g_type_init();

	#ifdef ENABLE_NLS
        /* Setting up gettext */
        bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
        textdomain(GETTEXT_PACKAGE);
        #endif /* ENABLE_NLS */

	app = g_object_new(TTB_TYPE_APP, NULL);

	ttb_app_exec(app, argc, argv);

	g_object_unref(app);

	return 1;
}

