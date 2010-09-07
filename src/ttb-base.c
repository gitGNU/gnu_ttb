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

#include "ttb-base.h"

#define TTB_BASE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
                                   TTB_TYPE_BASE, TTBBasePrivate))

G_DEFINE_TYPE(TTBBase, ttb_base, G_TYPE_OBJECT);

enum {
	FIELD_NAME,
	FIELD_EXEC,
	FIELD_ICON
};

struct _TTBBasePrivate
{
	GSList *list;
};

/* Eliminates %-values from input string.
 * Returns new string, which should be g_free'd; frees input string */
gchar*
ttb_base_unquote_exec(gchar *input)
{
	gchar *it = input;
	gchar *output = NULL;
	gchar buf[128];
	int i = 0;
	do {
		switch (*it) {
		case '%':
			++it;
			if G_UNLIKELY(*it == '%')
				break;
			continue;
		}
		buf[i++] = *it;
		if G_LIKELY(i != 127)
			continue;
		buf[i++] = 0;
		i = 0;
		gchar *t = output;
		if (t) {
			output = g_strjoin(NULL, output, buf, NULL);
			g_free(t);
		} else {
			output = g_strdup(buf);
		}
	} while (*(++it) != 0);

	if G_LIKELY(i != 0) {
		buf[i] = 0;
		gchar *t = output;
		if (t) {
			output = g_strjoin(NULL, output, buf, NULL);
			g_free(t);
		} else {
			output = g_strdup(buf);
		}

	}
	g_free(input);

	return output? output : g_strdup("");
}

static gboolean
ttb_base_load_keys_from_file(TTBBase *self, gchar *fname, GKeyFile *kfile)
{
	if (!g_key_file_load_from_file(kfile, fname, G_KEY_FILE_NONE, NULL))
		return FALSE;

	TTBBasePrivate *priv = self->priv;
	DesktopItem *item = g_malloc(sizeof(DesktopItem));

	gchar *val;

	val = g_key_file_get_string(kfile,
	                            G_KEY_FILE_DESKTOP_GROUP,
	                            G_KEY_FILE_DESKTOP_KEY_EXEC,
	                            NULL);
	if (val == NULL)
		return FALSE;
	item->exec = ttb_base_unquote_exec(val);
	
	val = g_key_file_get_string(kfile,
	                            G_KEY_FILE_DESKTOP_GROUP,
	                            G_KEY_FILE_DESKTOP_KEY_NAME,
	                            NULL);

	item->name = val? val : g_strdup("");

	val = g_key_file_get_string(kfile,
	                            G_KEY_FILE_DESKTOP_GROUP,
	                            G_KEY_FILE_DESKTOP_KEY_ICON,
	                            NULL);
	item->icon = val? val : g_strdup("");

	priv->list = g_slist_append(priv->list, item);
	return TRUE;
}


void
ttb_base_load_from_dir(TTBBase *self, gchar *dirname)
{
	TTBBaseClass *klass = TTB_BASE_GET_CLASS(self);
	klass->load_from_dir(self, dirname);
}

static void
load_from_dir(TTBBase *self, gchar *dirname)
{
	TTBBaseClass *klass = TTB_BASE_GET_CLASS(self);
	GDir *dir;
	const gchar *name;
	GKeyFile *kfile = g_key_file_new();
	TTBBasePrivate *priv = self->priv;

	/* In case given directory doesn't exist - create it */
	g_mkdir_with_parents(dirname, 0755);
	dir = g_dir_open(dirname, 0, NULL);
	while (name = g_dir_read_name(dir)) {
		gchar *dfname = g_build_filename(dirname, name, NULL);
		klass->load_keys_from_file(self, dfname, kfile);	
		g_free(dfname);
	}
	g_key_file_free(kfile);
}

static void
ttb_base_init(TTBBase *self)
{
	TTBBasePrivate *priv;
	self->priv = priv = TTB_BASE_GET_PRIVATE(self);
	priv->list = NULL;
}

static void
ttb_base_dispose(GObject *gobject)
{
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_base_parent_class)->dispose(gobject);
}

static void
ttb_base_finalize(GObject *gobject)
{
	TTBBase *self = TTB_BASE(gobject);

	GSList *list = self->priv->list;
	while(list) {
		DesktopItem *item = list->data;
		g_free(item->name);
		g_free(item->exec);
		g_free(item->icon);
		g_free(item);
		list = g_slist_next(list);
	}
	g_slist_free(self->priv->list);
	
	/* Chain up to the parent class */
	G_OBJECT_CLASS(ttb_base_parent_class)->finalize(gobject);
}

GSList*
ttb_base_get_entries_list(TTBBase *self)
{
	g_return_if_fail(TTB_IS_BASE(self));

	return self->priv->list;
}

void
ttb_base_set_entries_list(TTBBase *self, GSList *list)
{
	g_return_if_fail(TTB_IS_BASE(self));

	self->priv->list = list;
}

DesktopItem*
ttb_base_get_entry(TTBBase *self, gint index)
{
	g_return_if_fail(TTB_IS_BASE(self));

	GSList *list = self->priv->list;
	list = g_slist_nth(list, index);
	return list->data;
}

void
ttb_base_execute(gchar *exec)
{
	g_spawn_command_line_async(exec, NULL);
}

static void
ttb_base_class_init(TTBBaseClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	
	gobject_class->dispose  = ttb_base_dispose;
	gobject_class->finalize = ttb_base_finalize;

	klass->get_entries_list    = ttb_base_get_entries_list;
	klass->set_entries_list    = ttb_base_set_entries_list;
	klass->get_entry           = ttb_base_get_entry;
	klass->load_from_dir       = load_from_dir;
	klass->execute             = ttb_base_execute;
	klass->load_keys_from_file = ttb_base_load_keys_from_file;

	g_type_class_add_private(klass, sizeof(TTBBasePrivate));
}

