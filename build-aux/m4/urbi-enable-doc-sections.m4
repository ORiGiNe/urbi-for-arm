## Copyright (C) 2010, 2012, Gostai S.A.S.
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


# URBI_ARG_ENABLE_DOC_SECTIONS(PLATFORMS)
# ---------------------------------------
# PLATFORMS is a comma-separated list of platforms.
AC_DEFUN([URBI_ARG_ENABLE_DOC_SECTIONS],
[URBI_ARGLIST_ENABLE([enable-doc-sections=SECTIONS],
                     [generate the documentation],
                     [m4_join([|],
                              [all|yes],
                              [no|none],
                              [platforms],
                              $1)],
                     [all],
                     [
      SECTIONS: comma-separated list of:
        - all: build them all
        - platforms: generate the Platforms part
        - $1:
          include the corresponding chapter in the Platforms part
          and enable the Platforms part
        - no: alias for none
        - none: build none of them
        - yes: alias for all
])

urbi_enable_doc_sections ()
{
  local section
  local all='m4_join([ ], $1)'
  for section
  do
    case $section in
      (all|yes)
        urbi_enable_doc_sections platforms $all;;

      (platforms)
        enable_doc_section_platforms=true;;

      (m4_join([|], $1))
        urbi_enable_doc_sections platforms
        eval "enable_doc_section_$section"=true;;

      (none|no)
        enable_doc_section_platforms=false
        local d
        for d in platforms $all
        do
          eval "enable_doc_section_$d=false"
        done;;
    esac
  done
}

urbi_enable_doc_sections all $(echo $enable_doc_sections | tr ',' ' ')
])


# URBI_ENABLE_DOC_SECTIONS(PLATFORMS)
# -----------------------------------
# PLATFORMS is a comma-separated list of platforms.  It is presented as
# is to configure --help, so do something like this:
#
#  URBI_ENABLE_DOC_SECTIONS([bioloid, nao, nxt, p3dx, rmp, spykee, webots])
AC_DEFUN([URBI_ENABLE_DOC_SECTIONS],
[AC_MSG_CHECKING([for documentation sections])

URBI_ARG_ENABLE_DOC_SECTIONS([$1])

# Compute a summary for sake of AC_MSG_RESULT.
urbi_doc_sections=
enable_doc_sections=
for section in platforms m4_join([ ], $1)
do
  eval "ac_val=\$enable_doc_section_$section"
  if "$ac_val"; then
    urbi_doc_sections="$urbi_doc_sections $section"
  fi
  enable_doc_sections="$enable_doc_sections\
\\newboolean[[$ac_val]]{$section}"
done

AC_MSG_RESULT([$urbi_doc_sections])
AC_SUBST([enable_doc_sections])
])

## Local Variables:
## mode: autoconf
## End:
