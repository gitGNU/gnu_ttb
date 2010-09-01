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

#include "ttb-fbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gstdio.h>

#define TTB_FBASE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                    TTB_TYPE_FBASE, TTBFBasePrivate))
#define FILE_EXISTS(name) (g_file_test(name, G_FILE_TEST_EXISTS \
                                       | G_FILE_TEST_IS_DIR))


G_DEFINE_TYPE(TTBFBase, ttb_fbase, TTB_TYPE_BASE);

struct _TTBFBasePrivate
{
	GSList *list;
	GSList *remove; /* List of elements to be removed */
	gchar  *dirname;
};

static void
ttb_fbase_init(TTBFBase *self)
{
	TTBFBasePrivate *priv;
	self->priv = priv = TTB_FBASE_GET_PRIVATE(self);
	priv->list = NULL;
	priv->remove = NULL;
	priv->dirname = g_strdup("");
	g_random_set_seed(time(NULL));
}

static void
ttb_fbase_dispose(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_fbase_parent_class)->dispose(gobject);
}

static void
ttb_fbase_finalize(GObject *gobject)
{
	TTBFBase *self = TTB_FBASE(gobject);
	TTBFBasePrivate *priv = self->priv;

	GSList *list = priv->list;
	while(list) {
		g_free(list->data);
		list = g_slist_next(list);
	}
	g_slist_free(priv->list);

	list = priv->remove;
	while(list) {
		g_free(list->data);
		list = g_slist_next(list);
	}
	g_slist_free(priv->list);
	
	g_free(priv->dirname);

	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_fbase_parent_class)->finalize(gobject);
}

static gboolean
load_keys_from_file(TTBBase *self, gchar *fname, GKeyFile *kfile)
{
	g_return_if_fail(TTB_IS_FBASE(self));

	TTBBaseClass *klass = TTB_BASE_CLASS(ttb_fbase_parent_class);
	TTBFBasePrivate *priv = TTB_FBASE(self)->priv;

	if (!klass->load_keys_from_file(self, fname, kfile))
		return FALSE;

	priv->list = g_slist_append(priv->list, g_strdup(fname));
}

static void
load_from_dir(TTBBase *self, gchar *dirname)
{
	g_return_if_fail(TTB_IS_FBASE(self));

	TTBBaseClass *klass = TTB_BASE_CLASS(ttb_fbase_parent_class);
	TTBFBasePrivate *priv = TTB_FBASE(self)->priv;

	/* Recording dirname */
	g_free(priv->dirname);
	priv->dirname = g_strdup(dirname);

	klass->load_from_dir(self, dirname);
}

static gchar*
make_name(gchar *dirname, gchar *name)
{
	gchar *fname = g_strconcat(name, ".desktop", NULL);
	gchar *result = g_build_filename(dirname, fname, NULL);
	g_free(fname);
	if (FILE_EXISTS(result) || strlen(name) == 0) {
		g_free(result);
		gint digit = g_random_int_range(0, 10);
		fname = g_strdup_printf("%s%d", name, digit);
		result = make_name(dirname, fname);
	}
		
	return result;
}

/* FIXME: Make it return gboolean so we know if adding an entry was succesful
 * before messing with filename list */
static void
add_entry(TTBBase *self, gchar *name, gchar *exec, gchar *icon)
{
	g_return_if_fail(TTB_IS_FBASE(self));

	TTBBaseClass *klass = TTB_BASE_CLASS(ttb_fbase_parent_class);
	TTBFBasePrivate *priv = TTB_FBASE(self)->priv;

	priv->list = g_slist_append(priv->list, NULL);

	klass->add_entry(self, name, exec, icon);
}

static void
remove_entry(TTBBase *self, gint index)
{
	g_return_if_fail(TTB_IS_FBASE(self));

	TTBBaseClass *klass = TTB_BASE_CLASS(ttb_fbase_parent_class);
	TTBFBasePrivate *priv = TTB_FBASE(self)->priv;

	GSList *entry = g_slist_nth(priv->list, index);
	if (!entry)
		return;
	priv->remove = g_slist_append(priv->remove, g_strdup(entry->data));
	priv->list = g_slist_delete_link(priv->list, entry);

	klass->remove_entry(self, index);
}

/* TODO: Error handling and reporting */
static gboolean
save_file(const char *fname, DesktopItem *item)
{
	GKeyFile *kfile = g_key_file_new();	
	if (FILE_EXISTS(fname)) {
		g_key_file_load_from_file(kfile, fname,
		                          G_KEY_FILE_KEEP_COMMENTS
		                          | G_KEY_FILE_KEEP_TRANSLATIONS,
		                          NULL);
	}

	g_key_file_set_value(kfile,
	                     G_KEY_FILE_DESKTOP_GROUP,
	                     G_KEY_FILE_DESKTOP_KEY_EXEC,
	                     item->exec);
	g_key_file_set_value(kfile,
	                     G_KEY_FILE_DESKTOP_GROUP,
	                     G_KEY_FILE_DESKTOP_KEY_NAME,
	                     item->name);
	g_key_file_set_value(kfile,
	                     G_KEY_FILE_DESKTOP_GROUP,
	                     G_KEY_FILE_DESKTOP_KEY_ICON,
	                     item->icon);
	
	gint length;
	gchar *data = g_key_file_to_data(kfile, &length, NULL);
	g_key_file_free(kfile);
	g_file_set_contents(fname, data, length, NULL);
	g_free(data);

	return TRUE;
}

void
ttb_fbase_save(TTBFBase *self)
{
	g_return_if_fail(TTB_IS_FBASE(self));

	GSList *flist = self->priv->list;
	GSList *dlist = ttb_base_get_entries_list(TTB_BASE(self));
	while(flist && dlist) {
		gchar       *fname = flist->data;
		DesktopItem *item  = dlist->data;
		flist = g_slist_next(flist);
		dlist = g_slist_next(dlist);

		if (!fname)
			fname = make_name(self->priv->dirname, item->name);
		if (!save_file(fname, item))
			g_warning("Saving failed: %s", fname);
	}
	flist = self->priv->remove;
	while(flist) {
		g_remove(flist->data);
		g_free(flist->data);
		flist = g_slist_next(flist);
	}
	g_slist_free(flist);
	self->priv->remove = NULL;
}

static void
ttb_fbase_class_init(TTBFBaseClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	TTBBaseClass *base_class = TTB_BASE_CLASS(klass);

	gobject_class->dispose  = ttb_fbase_dispose;
	gobject_class->finalize = ttb_fbase_finalize;

	base_class->load_keys_from_file = load_keys_from_file;
	base_class->load_from_dir       = load_from_dir;
	base_class->add_entry           = add_entry;
	base_class->remove_entry        = remove_entry;

	klass->save = ttb_fbase_save;

	g_type_class_add_private(klass, sizeof(TTBFBasePrivate));
}

