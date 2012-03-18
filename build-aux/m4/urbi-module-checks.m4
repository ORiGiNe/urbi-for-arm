## Copyright (C) 2010-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-module-checks.m4
## This file is part of build-aux.

# serial 2

# URBI_MODULE_DISABLE(WHY)
# ------------------------
# Put this in braces so that we can run
#
#   $HAVE_foo || URBI_MODULE_DISABLE([missing dependency: foo])
#
# without failure.
AC_DEFUN([URBI_MODULE_DISABLE],
[{
  AC_MSG_NOTICE([module m4_defn([URBI_MODULE_Current]) disabled: $1])
  urbi_enable_module=false
}])



# MODULE_AC_CHECK_HEADER(HEADER)
# ------------------------------
# Check if HEADER is present.  If it is not, disable the current
# module.
AC_DEFUN([MODULE_AC_CHECK_HEADER],
[
  AC_CHECK_HEADER([$1],
                  [], [URBI_MODULE_DISABLE([missing header: $1])])
])

# MODULE_AC_CHECK_LIB(NAME)
# -------------------------
# Check if libNAME is present, and define HAVE_LIB<NAME> to
# true/false.
#
# If it is not, disable the current module.
AC_DEFUN([MODULE_AC_CHECK_LIB],
[
  AC_CHECK_LIB([$1], [main],
               [HAVE_LIB$1=true],
               [HAVE_LIB$1=false
                URBI_MODULE_DISABLE([missing lib: $1])])
])

# MODULE_PKG_CHECK_MODULES(VAR-PREFIX, MODULES)
# ---------------------------------------------
# wrapper around PKG_CHECK_MODULES.
#
# Requires pkg-config.
#
# If it is not, disable the current module.
AC_DEFUN([MODULE_PKG_CHECK_MODULES],
[
   PKG_CHECK_MODULES([$1], [$2], [],
                     [URBI_MODULE_DISABLE([missing pkg modules: $2])])
])

# MODULE_HOST([PATTERN])
# ----------------------
# Only build the module on the given host.
#
# If it is not, disable the current module.
AC_DEFUN([MODULE_HOST],
[case $ac_cv_host in
 (*$1*) ;;
 (*) URBI_MODULE_DISABLE([host requested: $1]);;
esac
])

# MODULE_HOST([PATTERN])
# ----------------------
# Only build the module if not on the given host.
#
# If it is not, disable the current module.
AC_DEFUN([MODULE_NOT_HOST],
[case $ac_cv_host in
  (*$1*) URBI_MODULE_DISABLE([host rejected: $1]);;
esac
])


## Local Variables:
## mode: autoconf
## End:
