## Copyright (C) 2008-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-apple.m4
## This file is part of build-aux.

# URBI_APPLE
# ----------
# Look for apple macos. Define the automake conditional APPLE.
AC_DEFUN([URBI_APPLE],
[case $host_os in
     (*darwin*)
       AM_CONDITIONAL([APPLE], [true]);;
     (*)
       AM_CONDITIONAL([APPLE], [false]);;
esac
])
