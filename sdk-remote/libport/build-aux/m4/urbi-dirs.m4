## Copyright (C) 2006-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-dirs.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])


# URBI_BRANDDIRS
# --------------
# Define brand-dirs, which are similar to Automake's pkg-dirs, except
# that they are based on $(PACKAGE_BRAND) instead of $(PACKAGE).
AC_DEFUN([URBI_BRANDDIRS],
[
AC_SUBST([brandincludedir], ['${includedir}/${PACKAGE_BRAND}'])
AC_SUBST([brandlibdir], ['${libdir}/${PACKAGE_BRAND}'])
AC_SUBST([brandlibexecdir], ['${libexecdir}/${PACKAGE_BRAND}'])
AC_SUBST([brandsharedir], ['${datadir}/${PACKAGE_BRAND}'])
])

# URBI_DIRS(DEFAULT-URBI-ENV)
# ---------------------------
# See URBI_DIRS and URBI_KERNEL_DIRS below.
#
# DEFAULT-URBI-ENV should probably be something like "aibo", "webots"
# etc.  This macro *MUST* be invoked *AFTER* URBI_WITH_KERNEL
# (sdk/urbi-with-kernel.m4).
#
# Define the directory variables we are about to use:
#
# branddir	   = ${prefix}/${PACKAGE_BRAND}
#                    /usr/local/gostai
#
# kbranddir	   - Where the kernel is installed.
#                  = ${brandir} for kernels
#                  = ${URBI_KERNEL_PATH}/${PACKAGE_BRAND} for others
#                    /usr/local/gostai           via --with-urbi-kernel.
#
# urbiincludedir   - Where the UObject headers are installed
#                  = ${kernelincludedir}/urbi for kernels
#                    /usr/local/gostai/kernel/include/urbi
#                  = ${includedir}/urbi for others
#                    /usr/local/include/urbi
#
# envdir	   - Where the kernel libraries are installed.
#                  = ${brandlibdir}/${URBI_ENV}
#                    /usr/local/gostai/core
#
# kerneldir	   = ${kbranddir}/kernel/${URBI_HOST}/${URBI_ENV}
#                    /usr/local/gostai/kernel/aibo
#
# sdkincludedir	   = ${branddir}/KIND/include
#
# kernelincludedir = ${kbranddir}/kernel/include
AC_DEFUN([URBI_DIRS],
[# We need to know the host (the type of architecture it will run on),
# the environment (the runtime, the event loop: engine, webots, aibo).
AC_REQUIRE([URBI_CANONICAL_HOST])dnl
AC_REQUIRE([URBI_BRANDDIRS])dnl
# URBI_ENV
AC_ARG_ENABLE([env],
	      [AC_HELP_STRING([--enable-env=urbi-env],
			      [The environment this will run on:
			       aibo, webots, korebot, engine [$1]])])
AC_MSG_CHECKING([for Urbi environment type])
case $enable_env in
 ('') URBI_ENV=$1;;
  (*) URBI_ENV=$enable_env;;
esac
AC_MSG_RESULT([$URBI_ENV])
case $URBI_ENV in
  (aibo)
     AC_DEFINE([URBI_ENV_AIBO], [1], [Define if compiling for Aibo.]);;
  (korebot)
     AC_DEFINE([URBI_ENV_KOREBOT], [1],
	       [Define if compiling for Korebot-based robots.]);;
  (engine)
     AC_DEFINE([URBI_ENV_ENGINE], [1], [Define if compiling generic engine.]);;
  (webots)
     AC_DEFINE([URBI_ENV_WEBOTS], [1], [Define if compiling for Webots.]);;
  (*)
     AC_MSG_NOTICE([[unknown environment type: $URBI_ENV]]);;
esac
AC_SUBST([URBI_ENV])

# URBI_ROOT.
AC_DEFINE_UNQUOTED([URBI_ROOT],
                   ["$(URBI_RESOLVE_DIR(["$prefix"]))"],
                   [Define as the install prefix.])

# If we target windows, dealing with paths to dlls is a problem.
# Everything is much simpler if we just put the dlls in the bindir.
case $URBI_HOST in
   (*mingw*)
     libdir='${bindir}'
     AC_SUBST([libdirname], [bin])
     ;;
   (*)
     AC_SUBST([libdirname], [lib])
     ;;
esac
AC_DEFINE_UNQUOTED([LIBDIRNAME], ["$libdirname"],
                   [Defined to libdir's basename])

# Everything is installed in $URBI_KERNEL_PATH/gostai.
AC_SUBST([PACKAGE_BRAND], [gostai])
AC_SUBST([branddir], ['${prefix}/${PACKAGE_BRAND}'])
URBI_PACKAGE_KIND_SWITCH(
  [core],   [AC_SUBST([kbranddir], ['${URBI_KERNEL_PATH}/${PACKAGE_BRAND}'])],
  [kernel], [AC_SUBST([kbranddir], ['${branddir}'])])

# SDK-Remote's headers.
AC_SUBST([urbiincludedir], ['${includedir}/urbi'])

# /usr/local/lib/gostai/core
AC_SUBST([envdir],    ['${brandlibdir}/${URBI_ENV}'])
AC_SUBST([kerneldir], ['${brandlibdir}/${URBI_ENV}'])

# Where we install standard uobjects.
# /usr/local/lib/gostai/uobjects/urbi.
#  - lib/gostai
#    to keep a low foot-print on the install tree (we don't spread
#    too much, and we leave some room for additions).
#
#  - uobjects
#    because that's what it is.
#
#  - urbi/
#    so that loading standard modules is done via "loadModule("urbi/foo")",
#    avoiding gratuitious conflicts with the possible user's module foo.
AC_SUBST([uobjectsdir], ['${brandlibdir}/uobjects${LIBSFX}'])
AC_SUBST([urbi_uobjectsdir], ['${uobjectsdir}/urbi'])
AC_SUBST([test_uobjectsdir], ['${uobjectsdir}/test'])

# Where we install, and expect to find, headers.
URBI_PACKAGE_KIND_SWITCH([core\|sdk],
	[AC_SUBST([sdkincludedir], ['${brandincludedir}/core'])])
URBI_PACKAGE_KIND_SWITCH([sdk], [],
	[AC_SUBST([kernelincludedir], ['${brandincludedir}/kernel'])])

# When compiling a core, we need to find the kernel headers and libraries.
URBI_PACKAGE_KIND_SWITCH(
  [core],
   [AC_SUBST([KERNEL_CPPFLAGS], ['-I${kernelincludedir}'])
    AC_SUBST([KERNEL_LDFLAGS],  ['-L${envdir}'])],
  [sdk],
   [AC_SUBST([SDK_CPPFLAGS], ['-I${sdkincludedir} -I${kernelincludedir}'])
    AC_SUBST([SDK_LDFLAGS],  ['-L${envdir}'])])


# Where we install umain.cc.  Used by umake.
AC_SUBST([umaindir], ['${brandsharedir}/umain'])
])


## Local Variables:
## mode: autoconf
## End:
