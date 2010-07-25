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

#ifndef TTB_UI_H
#define TTB_UI_H

#include "ttb-base.h"

#define TTB_TYPE_UI            (ttb_ui_get_type())
#define TTB_UI(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                TTB_TYPE_UI, TTBUI))
#define TTB_IS_UI(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                TTB_TYPE_UI))
#define TTB_UI_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                TTB_TYPE_UI, TTBUIClass))
#define TTB_IS_UI_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                TTB_TYPE_UI))
#define TTB_UI_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                TTB_TYPE_UI, TTBUIClass))

G_BEGIN_DECLS

typedef struct _TTBUI        TTBUI;
typedef struct _TTBUIClass   TTBUIClass;
typedef struct _TTBUIPrivate TTBUIPrivate;

struct _TTBUI
{
	GObject parent_instance;

	/* instance members */
	TTBUIPrivate *priv;
};

struct _TTBUIClass
{
	GObjectClass parent_class;

	/* class members */
	TTBBase*  (*get_base)(TTBUI *self);
	gboolean (*exec)(TTBUI *self, int argc, char **argv); /* virtual */
	void (*rebuild)(TTBUI *self);                         /* virtual */
};

/* used by TTB_TYPE_UI */
GType ttb_ui_get_type(void);

/*
 * Method definitions.
 */

TTBBase* ttb_ui_get_base(TTBUI *self);

void ttb_ui_exec(TTBUI *self, int argc, char **argv);

void ttb_ui_rebuild(TTBUI *self);

G_END_DECLS

#endif /* TTB_UI_H */
