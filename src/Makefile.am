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

bin_PROGRAMS = ttb ttb-prefs

nobase_dist_pkgdata_DATA = glade/prefs.ui

AM_CFLAGS = $(deps_CFLAGS)

ttb_LDADD = $(deps_LIBS)

ttb_SOURCES = main.c \
		ttb-base.c \
		ttb-base.h \
		ttb-ui.c \
		ttb-ui.h \
		ui-gtk.c \
		ui-gtk.h

ttb_prefs_LDADD = $(deps_LIBS)

ttb_prefs_SOURCES = gtk-prefs-main.c \
		ttb-base.c \
		ttb-base.h \
		ttb-fbase.h \
		ttb-fbase.c \
		ui-gtk-prefs.c \
		ui-gtk-prefs.h

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
	echo '#endif /* TTB_PATHS_H */' \
	) >$@

