# ===========================================================================
#    http://www.gnu.org/software/autoconf-archive/ax_check_java_home.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_JAVA_HOME
#
# DESCRIPTION
#
#   Check for Sun Java (JDK / JRE) installation, where the 'java' VM is in.
#   If found, set environment variable JAVA_HOME = Java installation home,
#   else left JAVA_HOME untouch, which in most case means JAVA_HOME is
#   empty.
#
# LICENSE
#
#   Copyright (c) 2008, 2010, 2011 Gleen Salmon <gleensalmon@yahoo.com>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

# serial 8

AU_ALIAS([AC_CHECK_JAVA_HOME], [AX_CHECK_JAVA_HOME])


# _AX_CHECK_JAVA_HOME_FOLLOW_SYMLINKS(PATH)
# -----------------------------------------
# Follows symbolic links on PATH, set the result in
# ax_check_java_home_follow_symlinks_result.
AC_DEFUN([_AX_CHECK_JAVA_HOME_FOLLOW_SYMLINKS],
[ax_cur="$1"
while ls -ld "$ax_cur" 2>/dev/null | grep " -> " >/dev/null;
do
  ax_slink=`ls -ld "$ax_cur" | sed 's/.* -> //'`
  case $ax_slink in
   /*) ax_cur=$ax_slink;;
    *) # 'X' avoids triggering unwanted echo options.
       ax_cur=`echo "X$ax_cur" | sed -e 's/^X//;s:[[^/]]*$::'`"$ax_slink";;
  esac
done
ax_check_java_home_follow_symlinks_result=$ax_cur
])


# AX_CHECK_JAVA_HOME
# ------------------
AC_DEFUN([AX_CHECK_JAVA_HOME],
[AC_PATH_PROG([JAVAC_PATH_NAME], [javac])

AC_MSG_CHECKING([for JAVA_HOME])
# We used a fake loop so that we can use "break" to exit when the result
# is found.
while true
do
  # If the user defined JAVA_HOME, don't touch it.
  test "${JAVA_HOME+set}" = set && break

  # On Mac OS X 10.5 and following, run /usr/libexec/java_home to get
  # the value of JAVA_HOME to use.
  # (http://developer.apple.com/library/mac/#qa/qa2001/qa1170.html).
  JAVA_HOME=`/usr/libexec/java_home 2>/dev/null`
  test x"$JAVA_HOME" != x && break

  # For instance on Gentoo.
  JAVA_HOME=`java-config --jdk-home 2>/dev/null`
  test x"$JAVA_HOME" != x && break

  # See if we can find the javac executable, and compute from there.
  _AX_CHECK_JAVA_HOME_FOLLOW_SYMLINKS([$JAVAC_PATH_NAME])
  if test "x$ax_check_java_home_follow_symlinks_result" != x; then
    JAVA_HOME=`echo $ax_check_java_home_follow_symlinks_result |
               sed 's,/bin/javac.*$,,'`
    break
  fi

  AC_MSG_NOTICE([cannot compute JAVA_HOME])
  break
done
AC_MSG_RESULT([$JAVA_HOME])
])

# Local Variables:
# mode: autoconf
# End:
