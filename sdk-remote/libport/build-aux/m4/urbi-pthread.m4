#						-*- Autoconf -*-

## Copyright (C) 2006-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-pthread.m4
## This file is part of build-aux.

# URBI_PTHREAD
# ------------
# Look for the pthreads.
AC_DEFUN_ONCE([URBI_PTHREAD],
[AC_REQUIRE([URBI_WIN32])dnl
ACX_PTHREAD([pthreads=true], [pthreads=false])
AC_CHECK_HEADERS([pthread.h])

URBI_APPEND_FLAGS([SDK_CFLAGS],   [$PTHREAD_CFLAGS])
URBI_APPEND_FLAGS([SDK_CXXFLAGS], [$PTHREAD_CFLAGS])
URBI_APPEND_FLAGS([SDK_LIBS],     [$PTHREAD_LIBS])
if ! $windows; then
   URBI_APPEND_FLAGS([PTHREAD_LDFLAGS], [-lpthread])
fi
AC_SUBST([PTHREAD_LDFLAGS])
])
