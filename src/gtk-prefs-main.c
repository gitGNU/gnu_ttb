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

#include <stdio.h>
#include "ttb-fbase.h"
#include "ui-gtk-prefs.h"
#include "config.h"
#include "ttb-paths.h"

int main(int argc, char **argv)
{
	g_type_init();

	gchar *dirname = g_build_filename(g_get_home_dir(),
	                                  ".local/share/applications/ttb",
	                                  NULL);
	TTBBase *base = g_object_new(TTB_TYPE_FBASE, NULL);
	ttb_base_load_from_dir(base, dirname);
	g_free(dirname);

	#ifdef ENABLE_NLS
	/* Setting up gettext */
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	textdomain(GETTEXT_PACKAGE);
	#endif /* ENABLE_NLS */

	gtk_init(&argc, &argv);

	UIGtkPrefs *prefs = g_object_new(UI_TYPE_GTK_PREFS, "base", base,
	                                 NULL);
	if (argc == 2) {
		int pid = atoi(argv[1]);
		ui_gtk_prefs_set_pid_of_ttb(prefs, pid);
	}
	ui_gtk_prefs_show_prefs(prefs);

	gtk_main();

	g_object_unref(prefs);
	g_object_unref(base);

	return 1;
}

