## Process this file with automake to produce Makefile.in
##
## Copyright (C) 2010, Mariusz Adamski <mariusz.adamski@gmail.com>
##
## TabletToolbox is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## TabletToolbox is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with TabletToolbox. If not, see <http://www.gnu.org/licenses/>.

bin_PROGRAMS = ttb

nobase_dist_pkgdata_DATA = glade/prefs.glade

modulesdir = $(libdir)/ttb-0.1/modules

modules_LTLIBRARIES = gtk-prefs.la

AM_CFLAGS = $(deps_CFLAGS) -DMODULESDIR=\""$(modulesdir)"\"

ttb_LDADD = $(deps_LIBS) libcommon.la

ttb_SOURCES = main.c \
		ttb-app.c \
		ttb-app.h \
		ttb-base.c \
		ttb-base.h \
		ttb-ui.c \
		ttb-ui.h \
		ui-gtk.c \
		ui-gtk.h

gtk_prefs_la_SOURCES = gtk-prefs-module.c \
		gtk-prefs-module.h \
		ttb-fbase.h \
		ttb-fbase.c

gtk_prefs_la_LIBADD = $(deps_LIBS) libcommon.la

gtk_prefs_la_LDFLAGS = -avoid-version -module

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = ttb-prefs.c \
		ttb-prefs.h \
		ttb-module.c \
		ttb-module.h

BUILT_SOURCES = ttb-paths.h

ttb-paths.h: Makefile
	@echo 'Creating $@'
	@( \
	echo '/* This file has been automatically generated. */'; \
	echo ''; \
	echo '#ifndef TTB_PATHS_H'; \
	echo '#define TTB_PATHS_H'; \
	echo ''; \
	echo '#define datadir "$(pkgdatadir)/"'; \
	echo '#define bindir "$(bindir)/"'; \
	echo '#define LOCALEDIR "$(localedir)/"'; \
	echo '#endif /* TTB_PATHS_H */' \
	) >$@

DISTCLEANFILES = ttb-paths.h

