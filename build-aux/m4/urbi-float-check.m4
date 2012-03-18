## Copyright (C) 2007-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-float-check.m4
## This file is part of build-aux.

# URBI_FLOAT_CHECK
# ----------------
# Look for various standard functions related to float that might not be
# portable (such as round).
AC_DEFUN([URBI_FLOAT_CHECK],
[
  AC_LANG_PUSH([C++])

  # On Aibo (mipsel-linux-g++) we don't have `round' and `trunc' so we must be
  # able to provide replacement implementations if needed.
  AC_CHECK_FUNCS([round])
  AC_CHECK_FUNCS([trunc])

  AC_LANG_POP([C++])
])

## Local Variables:
## mode: autoconf
## End:
