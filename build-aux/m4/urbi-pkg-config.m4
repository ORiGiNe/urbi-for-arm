## Copyright (C) 2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-pkg-config.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_PKG_CONFIG_VERSION(PACKAGE)
# --------------------------------
# Get the version of PACKAGE from pkg-config, assign it to PACKAGE_VERSION.
#
# For instance URBI_PKG_CONFIG_VERSION([ortp]) defines $ORTP_VERSION.
AC_DEFUN([URBI_PKG_CONFIG_VERSION],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
AC_MSG_CHECKING([for version of $1])
AS_VAR_PUSHDEF([Package_Version], AS_TR_CPP([$1_VERSION]))dnl
if test -n "$PKG_CONFIG"; then
  AC_SUBST(Package_Version, [$($PKG_CONFIG --modversion "$1")])
fi
AC_MSG_RESULT($Package_Version)
AS_VAR_POPDEF([Package_Version])
])

## Local Variables:
## mode: Autoconf
## End:
