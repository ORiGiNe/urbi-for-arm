#
# urbi-append-flags.m4: This file is part of build-aux.
# Copyright (C) 2010-2011, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.org
#

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_LIB_SUFFIX()
# -----------------
# Add option --enable-library-suffix=(auto|none|<value>).
#
# Once done, replace all  'mylib_la_* =' with 'mylib@LIBSFX@_la_* =',
# and all usage of 'mylib_la' with 'mylib$(LIBSFX)_la'.
AC_DEFUN([URBI_LIB_SUFFIX],
[
  urbi_lib_guess_suffix ()
  {
    local use_compiler_tag=$[1]

    local libsuffix
    case $urbi_compilation_mode in
      (*debug*) libsuffix=-d;;
      (*)       libsuffix=;;
    esac
    if "$use_compiler_tag"; then
      local tag
      tag=$(echo $boost_cv_lib_tag | sed -e 's/ .*//')
      if test -n "$tag"; then
        libsuffix=-$tag$libsuffix
      fi
    fi
    echo $libsuffix
  }

  urbi_lib_set_suffix ()
  {
    AC_DEFINE_UNQUOTED([LIBSFX], ["$[1]"],
                       [Define to the library suffix.])
    AC_SUBST([LIBSFX], [$[1]])
  }

  URBI_ARG_ENABLE([enable-library-suffix=SFX],
                  [the library suffix to use],
                  [[auto|autodebug|none|[-_a-zA-Z0-9]*]],
                  [none], [
     SFX (library suffix)
       - auto or yes or '': LIBSFX is computed using the debug mode (-d)
          and the compiler tag.
       - autodebug: only debug mode is used.
       - none: no suffix at all
       - *: use the passed argument as suffix])
  AC_MSG_CHECKING([for library suffix])
  case $enable_library_suffix in
    (auto|yes)      sfx=$(urbi_lib_guess_suffix true) ;;
    (autodebug)     sfx=$(urbi_lib_guess_suffix false) ;;
    (none|false|no) sfx= ;;
    (*)             sfx=$enable_library_suffix ;;
  esac
  AC_MSG_RESULT([$sfx])
  urbi_lib_set_suffix "$sfx"
])


## Local Variables:
## mode: Autoconf
## End:
