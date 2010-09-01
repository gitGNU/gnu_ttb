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

#ifndef UI_GTK_PREFS_H
#define UI_GTK_PREFS_H

#include <gtk/gtk.h>

#define UI_TYPE_GTK_PREFS            (ui_gtk_prefs_get_type())
#define UI_GTK_PREFS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      UI_TYPE_GTK_PREFS, UIGtkPrefs))
#define UI_IS_GTK_PREFS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      UI_TYPE_GTK_PREFS))
#define UI_GTK_PREFS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                      UI_TYPE_GTK_PREFS, UIGtkPrefsClass))
#define UI_IS_GTK_PREFS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                      UI_TYPE_GTK_PREFS))
#define UI_GTK_PREFS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                      UI_TYPE_GTK_PREFS, UIGtkPrefsClass))

G_BEGIN_DECLS

typedef struct _UIGtkPrefs        UIGtkPrefs;
typedef struct _UIGtkPrefsClass   UIGtkPrefsClass;
typedef struct _UIGtkPrefsPrivate UIGtkPrefsPrivate;

struct _UIGtkPrefs
{
	GObject parent_instance;

	/* instance members */
	UIGtkPrefsPrivate *priv;
};

struct _UIGtkPrefsClass
{
	GObjectClass parent_class;

	/* class members */
	void (*show_prefs)(UIGtkPrefs *self);
	void (*set_pid_of_ttb)(UIGtkPrefs *self, int pid);
};

/* used by UI_TYPE_GTK_PREFS */
GType ui_gtk_prefs_get_type(void);

/*
 * Method definitions.
 */

void ui_gtk_prefs_show_prefs(UIGtkPrefs *self);

void ui_gtk_prefs_set_pid_of_ttb(UIGtkPrefs *self, int pid);

G_BEGIN_DECLS

#endif /* UI_GTK_PREFS_H */
