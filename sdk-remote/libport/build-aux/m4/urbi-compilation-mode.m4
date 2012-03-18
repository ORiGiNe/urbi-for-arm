## Copyright (C) 2008-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-compilation-mode.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])
m4_pattern_forbid([^TC_])

AC_PREREQ([2.60])

AC_DEFUN([URBI_COMPILATION_MODE],
[AC_BEFORE([$0], [URBI_PROG_CXX])dnl
# Compilation mode.
urbi_compilation_mode_set ()
{
  local mode
  for mode
  do
    case $mode in
      (build)
        URBI_APPEND_COMPILERFLAGS([-O0])
        ;;

      (cov)
        URBI_APPEND_COMPILERFLAGS([-fprofile-arcs -ftest-coverage])
        ;;

      (debug)
        COMPILATION_MODE_DEBUG=true
        URBI_APPEND_COMPILERFLAGS([[-O0], [-fno-inline], [-g], [-ggdb]])
        AC_DEFINE([COMPILATION_MODE_DEBUG], [1],
                  [Define to enable Urbi debugging tools.])
        # Not all the code includes config.h.
        URBI_APPEND_CPPFLAGS([-DCOMPILATION_MODE_DEBUG])

        # Define USE_VALGRIND only if valgrind/valgrind.h exists.
        AC_CHECK_HEADERS([valgrind/valgrind.h],
                         [URBI_APPEND_CPPFLAGS([-DUSE_VALGRIND])])
        ;;

      (final)
        urbi_compilation_mode_set ndebug symbols
        ;;

      (ndebug)
        URBI_APPEND_CPPFLAGS([-DNDEBUG])
        ;;

      (prof)
        URBI_APPEND_COMPILERFLAGS([-pg])
        ;;

      (space)
        COMPILATION_MODE_SPACE=true
        AC_DEFINE([COMPILATION_MODE_SPACE], [1],
                  [Define to 1 to optimize for space.])
        local i
        URBI_APPEND_COMPILERFLAGS([[-fomit-frame-pointer],
                                   [-fdata-sections]
                                   [-ffunction-sections]])
        TC_COMPILER_OPTION_IF([--gc-sections],
                              [URBI_APPEND_FLAGS([LDFLAGS], [--gc-sections])])
        urbi_compilation_mode_set final
        ;;

      (speed)
        URBI_APPEND_COMPILERFLAGS([-O3])
        COMPILATION_MODE_SPEED=true
        AC_DEFINE([COMPILATION_MODE_SPEED], [1],
                  [Define to 1 to optimize for speed the kernel
                   at the detriment of compilation time.])
        urbi_compilation_mode_set final
        ;;

      (symbols)
        AC_DEFINE([SYMBOLS_PRECOMPILED], [1],
                  [Define if Urbi symbols should be precompiled.])
        ;;

      (threads)
        AC_DEFINE([SCHED_CORO_OSTHREAD], [1],
                  [Define to use the OS-thread implementation of coroutines.])
        ;;

      (*)
        AC_MSG_ERROR([invalid compilation mode: $mode])
        ;;
    esac
  done

  # Whether we build for small space.
  AM_CONDITIONAL([COMPILATION_MODE_DEBUG], [$COMPILATION_MODE_DEBUG])
  AM_CONDITIONAL([COMPILATION_MODE_SPACE], [$COMPILATION_MODE_SPACE])
  AM_CONDITIONAL([COMPILATION_MODE_SPEED], [$COMPILATION_MODE_SPEED])
}

URBI_ARGLIST_ENABLE([enable-compilation-mode=MODE],
               [Compilation mode],
               [build|cov|debug|ndebug|final|prof|space|speed|symbols|threads],
               [debug],
               [
     MODE: comma-separated list of:
        Overall Levels:
          - build: Disable optimization for faster compilation.
          - debug: Link with debug runtime libraries,
                   enable debug information, and scanner/parser traces.
          - space: Optimize for smaller space (implies final).
          - speed: Optimize for speed (implies final).

        Finer grain components:
          - cov: Request code coverage instrumentation
          - ndebug: Define NDEBUG.
          - final: Implies ndebug and symbols
          - prof: Request profiling instrumentation
          - symbols: Activate precompiled-symbols.
          - threads: Implement coroutines with threads.])

AC_SUBST([COMPILATION_MODE_DEBUG], [false])
AC_SUBST([COMPILATION_MODE_SPACE], [false])
AC_SUBST([COMPILATION_MODE_SPEED], [false])
AC_MSG_CHECKING([for compilation mode])
urbi_compilation_mode=$(echo $enable_compilation_mode | tr ',' ' ')
AC_MSG_RESULT([$urbi_compilation_mode])
urbi_compilation_mode_set $urbi_compilation_mode
])

## Local Variables:
## mode: Autoconf
## End:
