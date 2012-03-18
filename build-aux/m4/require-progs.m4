## Copyright (C) 2009-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file urbi-doc.m4
## This file is part of build-aux.

m4_pattern_forbid([^URBI_])dnl
AC_PREREQ([2.60])

# URBI_REQUIRE_PROGS(VAR, CANDIDATES, PACKAGE)
# --------------------------------------------
AC_DEFUN([URBI_REQUIRE_PROGS],
[AC_CHECK_PROGS([$1], [$2], [no])
if test $$1 = no; then
  AC_MSG_ERROR([$2, from the $3 package, is needed])
fi
AC_SUBST([$1])
])


## Local Variables:
## mode: autoconf
## End:
