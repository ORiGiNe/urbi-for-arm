## Copyright (C) 2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-canonical-host.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_CANONICAL_HOST
# -------------------
# Compute:
# - URBI_HOST
#
# - URBI_HOST_CPU
#   arm|ppc|x86.
#
# - URBI_HOST_OS
#   linux|macos|windows
#
# - URBI_HOST_COMP
#   gcc4|msvc2005|msvc2008.
#
AC_DEFUN([URBI_CANONICAL_HOST],
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
# URBI_HOST
AC_ARG_ENABLE([host],
	      [AC_HELP_STRING([--enable-host=urbi-host],
			      [The machine this will run on [HOST]])])
AC_MSG_CHECKING([for Urbi host type])
case $enable_host:$host_alias in
  ('':'') URBI_HOST=$host;;
  ('':* ) URBI_HOST=$host_alias;;
  ( *:* ) URBI_HOST=$enable_host;;
esac
AC_MSG_RESULT([$URBI_HOST])
AC_SUBST([URBI_HOST])
AC_DEFINE_UNQUOTED([URBI_HOST], ["$URBI_HOST"],
                   [Define as the Urbi host architecture name.])

# URBI_HOST_CPU: arm|ppc|x86|x86_64.
URBI_HOST_CPU=$host_cpu
case $host_cpu in
  (i?86)   URBI_HOST_CPU=x86;;
  (x86_64) ;;
  (*)      AC_MSG_NOTICE([[unknown URBI_HOST_CPU: $URBI_HOST_CPU]]);;
esac
AC_SUBST([URBI_HOST_CPU])
AC_MSG_CHECKING([for URBI_HOST_CPU])
AC_MSG_RESULT([$URBI_HOST_CPU])

# URBI_HOST_OS: linux|macos|windows.
case $host_os in
  (darwin*)   URBI_HOST_OS=macos;;
  (mingw32)   URBI_HOST_OS=windows;;
  (linux-gnu) URBI_HOST_OS=linux;;
  (*)         URBI_HOST_OS=$host_os
              AC_MSG_NOTICE([[unknown URBI_HOST_OS: $URBI_HOST_OS]]);;
esac
AC_SUBST([URBI_HOST_OS])
AC_MSG_CHECKING([for URBI_HOST_OS])
AC_MSG_RESULT([$URBI_HOST_OS])

# URBI_HOST_COMP: gcc4|msvc2005|msvc2008.
case $CXX_FLAVOR in
  (gcc|gcc-apple)
    # There are many different outputs with --version, for instance:
    #
    # Debian: g++ (GCC) 4.1.2 20061115 (prerelease) (Debian 4.1.1-21)
    # Gentoo: g++ (Gentoo 4.4.1 p1.0) 4.4.1
    # MacOSX: i686-apple-darwin9-g++-4.0.1 (GCC) 4.0.1 (Apple Inc. build 5490)
    #
    # using -v seems safer:
    #
    # Debian: gcc version 4.1.2 20061115 (prerelease) (Debian 4.1.1-21)
    # Gentoo: gcc version 4.4.1 (Gentoo 4.4.1 p1.0)
    # MacOSX: gcc version 4.0.1 (Apple Inc. build 5490)
    #
    # but it's on stderr...
    urbi_host_comp=$($CXX -v 2>&1 |
                     perl -ne 'if (m{^gcc version (\d+)(?:\.\d+)+})
                               {
                                 print "$[1]\n";
                                 exit;
                                }')
    URBI_HOST_COMP=gcc$urbi_host_comp
    ;;
  (msvc)
    urbi_host_comp=$($CXX --version |
               perl -ne 'if (m{Microsoft .* Compiler Version (\d+)(?:\.\d+)*\s})
                         {
                           print "$[1]\n";
                           exit;
                         }')
    case $urbi_host_comp in
      (14) URBI_HOST_COMP=vcxx2005;;
      (15) URBI_HOST_COMP=vcxx2008;;
      (*)  URBI_HOST_COMP=vcxx$urbi_host_comp
           AC_MSG_NOTICE([[unknown MSVC++ version: $URBI_HOST_COMP]]);;
    esac
    ;;
  (*)
    URBI_HOST_COMP=$CXX
    AC_MSG_NOTICE([[unknown URBI_HOST_COMP: $URBI_HOST_COMP]])
    ;;
esac
AC_SUBST([URBI_HOST_COMP])
AC_MSG_CHECKING([for URBI_HOST_COMP])
AC_MSG_RESULT([$URBI_HOST_COMP])
])


## Local Variables:
## mode: autoconf
## End:
