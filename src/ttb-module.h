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

#ifndef TTB_MODULE_H
#define TTB_MODULE_H

#include <glib-object.h>
#include <gmodule.h>

#define TTB_TYPE_MODULE            (ttb_module_get_type())
#define TTB_MODULE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    TTB_TYPE_MODULE, TTBModule))
#define TTB_IS_MODULE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    TTB_TYPE_MODULE))
#define TTB_MODULE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),\
                                    TTB_TYPE_MODULE, TTBModuleClass))
#define TTB_IS_MODULE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),\
                                    TTB_TYPE_MODULE))
#define TTB_MODULE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),\
                                    TTB_TYPE_MODULE, TTBModuleClass))

G_BEGIN_DECLS

typedef struct _TTBModule        TTBModule;
typedef struct _TTBModuleClass   TTBModuleClass;

struct _TTBModule
{
	GTypeModule parent_instance;

	gchar   *filename;
	GModule *library;

	/* Module symbols */
	void (*load)(TTBModule *module);
	void (*unload)(TTBModule *module);
};

struct _TTBModuleClass
{
	GTypeModuleClass parent_class;
};

/* used by TTB_TYPE_MODULE */
GType      ttb_module_get_type(void) G_GNUC_CONST;

TTBModule* ttb_module_new(const char *filename);

/*
 * Method definitions.
 */

void ttb_module_load(TTBModule *module);

void ttb_module_unload(TTBModule *module);

G_END_DECLS

#endif /* TTB_MODULE_H */

