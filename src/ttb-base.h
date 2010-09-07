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

#ifndef TTB_BASE_H
#define TTB_BASE_H

#include <glib-object.h>

#define TTB_TYPE_BASE            (ttb_base_get_type())
#define TTB_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                  TTB_TYPE_BASE, TTBBase))
#define TTB_IS_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                  TTB_TYPE_BASE))
#define TTB_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                  TTB_TYPE_BASE, TTBBaseClass))
#define TTB_IS_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                  TTB_TYPE_BASE))
#define TTB_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                 TTB_TYPE_BASE, TTBBaseClass))

G_BEGIN_DECLS

typedef struct _TTBBase        TTBBase;
typedef struct _TTBBaseClass   TTBBaseClass;
typedef struct _TTBBasePrivate TTBBasePrivate;
typedef struct _DesktopItem    DesktopItem;

struct _TTBBase
{
	GObject parent_instance;

	/* instance members */
	TTBBasePrivate *priv;
};

struct _TTBBaseClass
{
	GObjectClass parent_class;

	/* class members */
	GSList* (*get_entries_list)(TTBBase *self);
	void (*set_entries_list)(TTBBase *self, GSList *list);
	DesktopItem* (*get_entry)(TTBBase *self, gint index);
	void (*load_from_dir)(TTBBase *self, gchar *dirname); /* virtual */
	void (*execute)(gchar *exec);
	/* protected */
	gboolean (*load_keys_from_file)(TTBBase *self, gchar *fname,
	                                GKeyFile *kfile);
};

struct _DesktopItem
{
	gchar *name;
	gchar *exec;
	gchar *icon;
};

/* used by TTB_TYPE_BASE */
GType ttb_base_get_type(void);

/*
 * Method definitions.
 */

GSList* ttb_base_get_entries_list(TTBBase *self);

void ttb_base_set_entries_list(TTBBase *self, GSList *list);

DesktopItem*
ttb_base_get_entry(TTBBase *self, gint index);

void ttb_base_execute(gchar *exec);

void ttb_base_load_from_dir(TTBBase *self, gchar *dirname);

G_END_DECLS

#endif /* TTB_BASE_H */
