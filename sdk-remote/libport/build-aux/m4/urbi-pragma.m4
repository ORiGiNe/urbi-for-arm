## Copyright (C) 2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-pragma.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.61])

# _URBI_PRAGMA_GCC_DIAGNOSTIC
# ---------------------------
# Compute urbi_cv_pragma_gcc_diagnostic.
AC_DEFUN([_URBI_PRAGMA_GCC_DIAGNOSTIC],
[urbi_pragma_cppflags=$CPPFLAGS
# On CPPFLAGS to work with C and C++.
CPPFLAGS="$CPPFLAGS -Wunknown-pragmas -Werror"
AC_COMPILE_IFELSE([AC_LANG_SOURCE(
      [[#pragma GCC diagnostic warning "-Wunused-parameter"
]])],
    [urbi_cv_pragma_gcc_diagnostic=yes],
    [urbi_cv_pragma_gcc_diagnostic=no])
CPPFLAGS=$urbi_pragma_cppflags
])

# URBI_PRAGMA_GCC_DIAGNOSTIC
# --------------------------
# Define `HAVE_PRAGMA_GCC_DIAGNOSTIC' (or not).
AC_DEFUN([URBI_PRAGMA_GCC_DIAGNOSTIC],
[AC_CACHE_CHECK([[whether the compiler supports @%:@pragma GCC diagnostic]],
                [urbi_cv_pragma_gcc_diagnostic],
                [_URBI_PRAGMA_GCC_DIAGNOSTIC])
if test x$urbi_cv_pragma_gcc_diagnostic = xyes; then
  AC_DEFINE([HAVE_PRAGMA_GCC_DIAGNOSTIC], 1,
            [Define to 1 if the compiler supports `@%:@pragma GCC diagnostic'.])
fi
])

## Local Variables:
## mode: autoconf
## End:
