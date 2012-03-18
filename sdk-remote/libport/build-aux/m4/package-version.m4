## Copyright (C) 2008-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-package-version.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_PACKAGE_VERSION([TOP-DIR])
# -------------------------------
# Use either TOP-DIR.version to recover the current version info, in
# order to avoid Autoconf-time dependencies on the version number.
#
# TOP-DIR should start with a slash, it will be appended to
# $(top_srcdir).  For instance URBI_PACKAGE_VERSION([/..]).
#
# Also define PACKAGE_MAJOR, PACKAGE_MINOR, and PACKAGE_PATCHLEVEL:
#
#   VERSION  MAJOR  MINOR  PATCHLEVEL
#    2.0      2       0      0
#    2.0.0    2       0      0
#    2.0.1    2       0      1
#    2.1      2       1      0
AC_DEFUN([URBI_PACKAGE_VERSION],
[AC_MSG_CHECKING([for current version])
# If the version is already defined, do nothing.
case $VERSION in (''|UNDEFINED)
  VERSION=$("$srcdir/build-aux/bin/versionify" -s "$srcdir$1" --file=-);;
esac
test -n "$VERSION" ||
  AC_MSG_ERROR([cannot get version information])
AC_MSG_RESULT([$VERSION])

urbi_define_versions ()
{
  local save_IFS="$IFS"
  IFS='.'
  set $PACKAGE_VERSION 0
  IFS=$save_IFS
  AC_SUBST([PACKAGE_MAJOR],      [$[1]])dnl
  AC_SUBST([PACKAGE_MINOR],      [$[2]])dnl
  AC_SUBST([PACKAGE_PATCHLEVEL], [$[3]])dnl
}
urbi_define_versions

# Change the definition of VERSION and PACKAGE_VERSION in the
# Makefiles.  This is run before creating config.status.
AC_CONFIG_COMMANDS_PRE([
# Use something which is more useful for the Makefiles.
VERSION='$(shell $(top_srcdir)/build-aux/bin/versionify --srcdir=$(top_srcdir)$1 --file=- --update)'

# We used to make PACKAGE_VERSION be '$(VERSION)', so that both are
# really the same.  But it is actually convenient to have
# PACKAGE_VERSION be exactly what is given in configure.ac (to compute
# the name of the documentation directory for instance), and have
# VERSION be more dynamic.
#
#PACKAGE_VERSION='$(VERSION)'
])
])

## Local Variables:
## mode: autoconf
## End:
