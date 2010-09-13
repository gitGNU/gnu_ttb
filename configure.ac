#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Copyright (C) 2010, Mariusz Adamski <mariusz.adamski@gmail.com>
#
# This configure.ac is free software; the author
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

AC_PREREQ([2.63])
AC_INIT([ttb], [0.0.1], [mariusz.adamski@gmail.com])
AM_INIT_AUTOMAKE([gnu dist-bzip2])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_GNU_GETTEXT([external])
IT_PROG_INTLTOOL

AC_MSG_CHECKING(whether to enable debugging)
debug_default="no"
AC_ARG_ENABLE(debug, [  --enable-debug=[no/yes] turn on debugging
                      [default=$debug_default]],,
              enable_debug=$debug_default)
if test "x$enable_debug" = "xyes"; then
	CFLAGS="$CFLAGS -g -DDEBUG"
	CXXFLAGS="$CXXFLAGS -g -DDEBUG"
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Checks for libraries.

ttb_deps="
	gobject-2.0
	gmodule-2.0
	gtk+-2.0
"
PKG_CHECK_MODULES(deps, $ttb_deps)

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_SUBST(GETTEXT_PACKAGE, ttb-${VERSION})
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
        [The package name, for gettext])
AC_CONFIG_FILES([Makefile po/Makefile.in
                 src/Makefile])
AC_OUTPUT

echo
echo "configure finished, now type make"
echo
