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

#ifndef TTB_FBASE_H
#define TTB_FBASE_H

#include <glib-object.h>
#include "ttb-base.h"

#define TTB_TYPE_FBASE            (ttb_fbase_get_type())
#define TTB_FBASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                   TTB_TYPE_FBASE, TTBFBase))
#define TTB_IS_FBASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                   TTB_TYPE_FBASE))
#define TTB_FBASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                   TTB_TYPE_FBASE, TTBFBaseClass))
#define TTB_IS_FBASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                   TTB_TYPE_FBASE))
#define TTB_FBASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                   TTB_TYPE_FBASE, TTBFBaseClass))

G_BEGIN_DECLS

typedef struct _TTBFBase        TTBFBase;
typedef struct _TTBFBaseClass   TTBFBaseClass;
typedef struct _TTBFBasePrivate TTBFBasePrivate;
typedef struct _FDesktopItem    FDesktopItem;

struct _TTBFBase
{
	TTBBase parent_instance;

	/* instance members */
	TTBFBasePrivate *priv;
};

struct _TTBFBaseClass
{
	TTBBaseClass parent_class;

	/* class members */
	void (*save)(TTBFBase *self);
};

/* used by TTB_TYPE_FBASE */
GType ttb_fbase_get_type(void);

/*
 * Method definitions.
 */

void ttb_fbase_save(TTBFBase *self);

G_END_DECLS

#endif /* TTB_BASE_H */
