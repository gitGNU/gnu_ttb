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

#ifndef UI_GTK_H
#define UI_GTK_H

#include "ttb-ui.h"
#include "ui-gtk-prefs.h"

#define UI_TYPE_GTK            (ui_gtk_get_type())
#define UI_GTK(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                UI_TYPE_GTK, UIGtk))
#define UI_IS_GTK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                UI_TYPE_GTK))
#define UI_GTK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                UI_TYPE_GTK, UIGtkClass))
#define UI_IS_GTK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                UI_TYPE_GTK))
#define UI_GTK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                UI_TYPE_GTK, UIGtkClass))

G_BEGIN_DECLS

typedef struct _UIGtk        UIGtk;
typedef struct _UIGtkClass   UIGtkClass;
typedef struct _UIGtkPrivate UIGtkPrivate;

struct _UIGtk
{
	TTBUI parent_instance;

	/* instance members */
	UIGtkPrefs *prefs;
	UIGtkPrivate *priv;
};

struct _UIGtkClass
{
	TTBUIClass parent_class;

	/* class members */
};

/* used by UI_TYPE_GTK */
GType ui_gtk_get_type(void);

/*
 * Method definitions.
 */

G_END_DECLS

#endif /* UI_GTK_H */
