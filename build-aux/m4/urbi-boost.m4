## Copyright (C) 2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-boost.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_BOOST_REQUIRE([VERSION = 1.38])
# ------------------------------------
# This macro should be called before URBI_LIBPORT.
AC_DEFUN([URBI_BOOST_REQUIRE],
[AC_BEFORE([$0], [_URBI_LIBPORT_BOOST])dnl
BOOST_REQUIRE(m4_default([$1], [1.38]))
if $COMPILATION_MODE_DEBUG; then
  boost_rt_opt=gd
fi

# Use only shlibs.  See autolinking feature used by Boost to require
# from the compiler to link specific libraries (yes, via special
# #pragmas in the headers).
case $CXX_FLAVOR in
  (msvc) URBI_APPEND_CPPFLAGS([-DBOOST_ALL_DYN_LINK]);;
esac

AC_SUBST([BOOST_VERSION], [$boost_cv_lib_version])
])

# URBI_BOOST(COMPONENT)
# ---------------------
# For instance "URBI_BOOST([TEST])" instead of BOOST_TEST.
AC_DEFUN([URBI_BOOST],
[AC_REQUIRE([URBI_BOOST_REQUIRE])dnl
BOOST_$1([$boost_rt_opt])
])


# URBI_BOOST_LOCATION
# -------------------
AC_DEFUN([URBI_BOOST_LOCATION],
[URBI_BOOST([SYSTEM])
# Where Boost headers live.
AC_MSG_CHECKING([for boost/version.hpp location])
_$0_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
# We remove the "/boost/version.hpp" part, but using a wildcard to
# cope with / and \ style paths.
_BOOST_SED_CPP([[/version.hpp/{
  s/\\\\\\(.\\)/\\1/g;
  s/^[^\"]*\"//;
  s/.boost.version\\.hpp\".*//g;
  p;
  q;
}]],
               [#include <boost/version.hpp>
],
    [BOOST_HEADER_DIR=`cat conftest.i`])
CPPFLAGS=$_$0_save_CPPFLAGS
AC_SUBST([BOOST_HEADER_DIR])
AC_MSG_RESULT([$BOOST_HEADER_DIR])

AC_MSG_CHECKING([for Boost libraries directory])
# If we did not need -L/-R to find them, let's bet they are in the
# sibling of the include directory.
test x"$BOOST_LDPATH" != x ||
  BOOST_LDPATH=${BOOST_HEADER_DIR%include}lib
AC_MSG_RESULT([$BOOST_LDPATH])

# Where the Boost libraries are.
AC_MSG_CHECKING([for Boost libraries pattern])
_$0_pattern=$(echo "$BOOST_SYSTEM_LIBS" |
              sed -e 's/^-l//;s/system/*/')

case $CXX_FLAVOR in
  (msvc)
    # DLLs must be copied with their corresponding LIB.
    # {}-globbing is not supported by all the /bin/sh.
    BOOST_LIBRARY_GLOB="$BOOST_LDPATH/${_$0_pattern}.dll $BOOST_LDPATH/${_$0_pattern}.lib"
    ;;
  (*)
    # shlibext includes the period.
    BOOST_LIBRARY_GLOB=$BOOST_LDPATH/lib${_$0_pattern}$SHLIBEXT
    ;;
esac
AC_SUBST([BOOST_LIBRARY_GLOB])
AC_MSG_RESULT([$BOOST_LIBRARY_GLOB])
])

## Local Variables:
## mode: Autoconf
## End:
