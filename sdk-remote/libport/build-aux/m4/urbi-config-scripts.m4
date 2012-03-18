## Copyright (C) 2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-config-scripts.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_CONFIG_SCRIPTS([SCRIPT1], [SCRIPT2:SOURCE2], ...)
# ------------------------------------------------------
# Same as AC_CONFIG_FILES, but runs chmod a+x.  Does not work well
# with shell lists: don't run
#
#  URBI_CONFIG_SCRIPTS([foo bar])
#
# but
#
#  URBI_CONFIG_SCRIPTS([foo], [bar])
#
# EXEEXT is available for use:
#
#  URBI_CONFIG_SCRIPTS([foo$EXEEXT:foo.in], [bar])
#
AC_DEFUN([URBI_CONFIG_SCRIPTS],
[m4_foreach([AC_Script], [$@],
[AC_CONFIG_FILES(AC_Script,
                 [chmod a+x "m4_bpatsubst(]AC_Script[, [:.*])"],
                 [EXEEXT=$EXEEXT])])
])


## Local Variables:
## mode: Autoconf
## End:
