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

# URBI_PROG_PDFLATEX([required])
# ------------------------------
# Set @PDFLATEX@ to the location of PDFLaTeX.  If "required" is passed
# as argument, die if not present.
AC_DEFUN([URBI_PROG_PDFLATEX],
[AC_CHECK_PROGS([PDFLATEX], [pdflatex], [no])
case "$1:$PDFLATEX" in
  (required:no)
    AC_MSG_ERROR([PDFLaTeX is required]);;
esac
AC_SUBST([PDFLATEX])
])


# URBI_PROG_PDFLATEX_REQUIRED
# ---------------------------
AC_DEFUN([URBI_PROG_PDFLATEX_REQUIRED],
[URBI_PROG_PDFLATEX([required])
])

AC_PREREQ([2.60])

## Local Variables:
## mode: autoconf
## End:
