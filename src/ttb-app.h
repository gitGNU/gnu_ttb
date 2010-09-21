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

#ifndef TTB_APP_H
#define TTB_APP_H

#include <glib-object.h>

#define TTB_TYPE_APP            (ttb_app_get_type())
#define TTB_APP(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                 TTB_TYPE_APP, TTBApp))
#define TTB_IS_APP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                 TTB_TYPE_APP))
#define TTB_APP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                 TTB_TYPE_APP, TTBAppClass))
#define TTB_IS_APP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                 TTB_TYPE_APP))
#define TTB_APP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                 TTB_TYPE_APP, TTBAppClass))

G_BEGIN_DECLS

typedef struct _TTBApp        TTBApp;
typedef struct _TTBAppClass   TTBAppClass;
typedef struct _TTBAppPrivate TTBAppPrivate;

struct _TTBApp
{
	GObject parent_instance;

	/* instance members */
	TTBAppPrivate *priv;
};

struct _TTBAppClass
{
	GObjectClass parent_class;

	/* class members */
};

/* used by TTB_TYPE_APP */
GType ttb_app_get_type(void);

/*
 * Method definitions.
 */

void ttb_app_run(TTBApp *self, int argc, char **argv);

TTBApp* ttb_app_get();

gpointer ttb_app_get_prefs(TTBApp *self);

G_END_DECLS

#endif /* TTB_APP_H */
