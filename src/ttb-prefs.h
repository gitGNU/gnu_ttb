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

#ifndef TTB_PREFS_H
#define TTB_PREFS_H

#include <gtk/gtk.h>
#include "ttb-base.h"
#include "ttb-ui.h"

#define TTB_TYPE_PREFS            (ttb_prefs_get_type())
#define TTB_PREFS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                   TTB_TYPE_PREFS, TTBPrefs))
#define TTB_IS_PREFS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                   TTB_TYPE_PREFS))
#define TTB_PREFS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                   TTB_TYPE_PREFS, TTBPrefsClass))
#define TTB_IS_PREFS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                   TTB_TYPE_PREFS))
#define TTB_PREFS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                   TTB_TYPE_PREFS, TTBPrefsClass))

G_BEGIN_DECLS

typedef struct _TTBPrefs        TTBPrefs;
typedef struct _TTBPrefsClass   TTBPrefsClass;

struct _TTBPrefs
{
	GObject parent_instance;

	/* instance members */
};

struct _TTBPrefsClass
{
	GObjectClass parent_class;

	/* class members */
	const gchar *name;
	gpointer (*get_widget)(TTBPrefs *self);
	void     (*add_dir)(TTBPrefs *self, const char *dirname);
	void     (*connect_ui)(TTBPrefs *self, TTBUI *ui);
};

/* used by UI_TYPE_PREFS */
GType ttb_prefs_get_type(void);

/*
 * Method definitions.
 */

gpointer ttb_prefs_get_widget(TTBPrefs *self);

void ttb_prefs_add_dir(TTBPrefs *self, const char *dirname);

void ttb_prefs_connect_ui(TTBPrefs *self, TTBUI *ui);

G_BEGIN_DECLS

#endif /* TTB_PREFS_H */
