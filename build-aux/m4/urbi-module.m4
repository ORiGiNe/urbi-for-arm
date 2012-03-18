## Copyright (C) 2009-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-module.m4
## This file is part of build-aux.

# This set of macro can be used to make a modular build system.
# Usage:
# * Include the .ac files of your modules, that must call
# URBI_MODULE(name, code). If code sets urbi_enable_module to false
# or if the user did not include the module in its list, the module will
# be disabled. BUILD_$name will be set in your .am files accordingly.
# * Call URBI_MODULAR_BUILD and URBI_MODULE_REPORT.

# serial 4

# URBI_MODULE(NAME, CODE)
# -----------------------
# Register module.
# If the module is not already disabled, call code, disable module if
# urbi_enable_module is false.
AC_DEFUN([URBI_MODULE],
[m4_pushdef([URBI_MODULE_Current], [$1])dnl
m4_append([URBI_modules], [$1], [,])dnl
AC_MSG_NOTICE([configuring module $1...])

urbi_enable_module=true
case ",$enable_modules," in
  (*,$1,*|,,) ;;
  (*) URBI_MODULE_DISABLE([not in --enable-modules]);;
esac

AS_IF([$urbi_enable_module], [$2])

AM_CONDITIONAL([BUILD_$1], [$urbi_enable_module])
urbi_module_$1_enabled=$urbi_enable_module
if $urbi_enable_module; then
  urbi_enabled_modules="$urbi_enabled_modules $1"
  urbi_module_res=enabled
else
  urbi_disabled_modules="$urbi_disabled_modules $1"
  urbi_module_res=disabled
fi

AC_MSG_NOTICE([configuring module $1... $urbi_module_res])
m4_popdef([URBI_MODULE_Current])dnl
])


# URBI_MODULE_FLAGS(CPPFLAGS, LDFLAGS, LIBS)
# ------------------------------------------
# Define foo_CPPFLAGS, foo_LDFLAGS, foo_LIBS for the current module
# foo.
#
# Note that the arguments are in alphabetical order, and logical order
# of dependencies.
AC_DEFUN([URBI_MODULE_FLAGS],
[AC_SUBST(URBI_MODULE_Current[_CPPFLAGS], [$1])dnl
AC_SUBST(URBI_MODULE_Current[_LDFLAGS],   [$2])dnl
AC_SUBST(URBI_MODULE_Current[_LIBS],      [$3])dnl
])


# URBI_MODULE_REPORT
# ------------------
# Display the lists of enabled and disabled modules.
AC_DEFUN([URBI_MODULE_REPORT],
[
  # urbi_sort VARIABLE
  # ------------------
  # Sort in place the contents of VARIABLE, a space separated list.
  urbi_sort ()
  {
    local var=$[1]
    local val
    eval "val=\$$var"
    val=$(perl -e 'print join (" ", sort @ARGV)' -- $val)
    eval "$var=\$val"
  }
  urbi_sort urbi_enabled_modules
  urbi_sort urbi_disabled_modules

  AC_MSG_NOTICE([Enabled modules: $urbi_enabled_modules])
  AC_MSG_NOTICE([Disabled modules:$urbi_disabled_modules])

  urbiscript_string_list ()
  {
    local res
    local i
    for i
    do
      if test -z "$res"; then
        res="\"$i\""
      else
        res="$res, \"$i\""
      fi
    done
    urbiscript_string_list_res="[[$res]]"
  }

  urbiscript_string_list $urbi_disabled_modules
  AC_SUBST([DISABLED_MODULES_U], [$urbiscript_string_list_res])
  AC_SUBST([DISABLED_MODULES], [$urbi_disabled_modules])
  urbiscript_string_list $urbi_enabled_modules
  AC_SUBST([ENABLED_MODULES_U], [$urbiscript_string_list_res])
  AC_SUBST([ENABLED_MODULES], [$urbi_enabled_modules])
])

# URBI_MODULAR_BUILD
# ------------------
# Add a --enable-modules=list configure option.
AC_DEFUN([URBI_MODULAR_BUILD],
[
  URBI_ARG_ENABLE([enable-modules],
    [Comma-separated list of modules to enable:],
    [.*],
    m4_defn([URBI_modules]),
    []
    )
])

## Local Variables:
## mode: autoconf
## End:
