## Copyright (C) 2006-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file init.mk
## This file is part of build-aux.

# Initialize some common variables so that we can always use +=.  Do
# not initialize TESTS, otherwise Automake sees empty test suites
# everywhere.

# Dependencies on "all".
ALLS =
AM_CFLAGS =
AM_CPPFLAGS =
AM_CXXFLAGS =
# We work on C++ only.
AM_DEFAULT_SOURCE_EXT = .cc
AM_LDFLAGS =
# Require 1.11.1 asap.
# http://lists.gnu.org/archive/html/automake/2009-12/msg00012.html
AUTOMAKE_OPTIONS = 1.11
# Use bzip2 only.
AUTOMAKE_OPTIONS += no-dist-gzip dist-bzip2
BUILDCHECK_ENVIRONMENT = $(CHECK_ENVIRONMENT)
BUILT_SOURCES =
CHECK_CLEANFILES =
CHECK_ENVIRONMENT =
CLEANFILES = $(BUILT_SOURCES)
CLEANDIRS =
DISTCLEANFILES =
EXTRA_DATA =
EXTRA_DIST =
EXTRA_HEADERS =
EXTRA_LTLIBRARIES =
INSTALLCHECK_ENVIRONMENT = $(CHECK_ENVIRONMENT)
LIBS =
M4SHFLAGS =
MAINTAINERCLEANFILES =
MOSTLYCLEANFILES =
SUFFIXES =
TESTS_ENVIRONMENT =
TFAIL_TESTS =
XFAIL_TESTS =
bin_PROGRAMS =
dist_bin_SCRIPTS =
dist_noinst_DATA =
dist_noinst_SCRIPTS =
include_HEADERS =
lib_LTLIBRARIES =
nodist_bin_SCRIPTS =
nodist_check_DATA =
nodist_check_SCRIPTS =
nodist_noinst_DATA =
nodist_noinst_SCRIPTS =
noinst_LTLIBRARIES =

# Because we use sysexists, which uses 77, we can't use the
# traditional 77 to mean SKIP tests.  Rather, use 176 consistently.
SKIP_STATUS = 176

# Visual Studio Debugger PDB Files (C++).
# http://msdn2.microsoft.com/en-us/library/yd4f8bd1.aspx.
CLEANFILES += vc80.pdb
# Files created by MSVC++ and our cl.exe.
CLEANFILES += $(PROGRAMS:=.auto.*) $(EXTRA_PROGRAMS:=.auto.*)

## FIXME: I don't understand how come these files are created.  It
## might be an incompability between our cl.exe wrapper and libtool:
## the files are created in buiddir instead of .libs/.  It does not
## even include the subdir/ component in subdir-objects.
CLEANFILES += lt-*.obj

# Files which are not shipped should be cleaned.
CLEANFILES += $(nodist_check_SCRIPTS) $(nodist_noinst_SCRIPTS)

# Automake still does not honor AM_LDADD.
LDADD = $(AM_LDADD)

## Useful variables.
build_aux_builddir = $(top_builddir)/build-aux
build_aux_dir = $(build_aux_srcdir)
build_aux_srcdir = $(top_srcdir)/build-aux
move_if_change = $(build_aux_dir)/bin/move-if-change
move_if_change_run = $(move_if_change) --color $(if $(V:0=),--verbose,--silent)

## The following uses GNU Make.
AUTOMAKE_OPTIONS += -Wno-portability

all: $(ALLS)

## ------- ##
## Clean.  ##
## ------- ##

# CLEAN_LOCAL: instead of hooking clean-local, which is troublesome
# when there are AM_CONDITIONAL used, append the target to
# CLEAN_LOCAL.
CLEAN_LOCAL = clean-cleandirs
.PHONY: $(CLEAN_LOCAL)
clean-local: $(CLEAN_LOCAL)

clean-cleandirs:
	-test -z "$(CLEANDIRS)" || rm -rf $(CLEANDIRS)


## ------------ ##
## Versionify.  ##
## ------------ ##

VERSIONIFY = $(top_srcdir)/build-aux/bin/versionify
VERSIONIFY_CACHE ?= $(top_srcdir)/.version
VERSIONIFY_PACKAGE ?= $(PACKAGE_TARNAME)
VERSIONIFYFLAGS =
VERSIONIFY_RUN =				\
  $(VERSIONIFY)					\
  --directory					\
  --cache=$(VERSIONIFY_CACHE)			\
  --package="$(VERSIONIFY_PACKAGE)"

# See an example of use in sdk-remote/src/Makefile.am.
AT_V_PATCH = $(AT_V_PATCH_$(V))
AT_V_PATCH_ = $(AT_V_PATCH_$(AM_DEFAULT_VERBOSITY))
AT_V_PATCH_0 = @echo "  PATCH " $^;


## -------- ##
## Stamps.  ##
## -------- ##

# Running continuously the test suite is costly, especially for the
# buildfarm.  Some changes have no impact whatsoever on the kernel,
# uconsole etc, for instance changes in the documentation.  In that
# case we would like not to rerun the test suite.  But spelling out
# the exact dependencies (on the libraries, the binaries, the
# wrappers, but also the urbi/*.u files) is hard.  So we create stamp
# files that make it easier to depend on changes of some components.
STAMPS =
.PHONY: stamps
stamps: $(STAMPS)
noinst_DATA = $(STAMPS)
CLEANFILES += $(STAMPS)
%.stamp:
	$(AM_V_GEN) echo "$$(date): $?" >>$@

## ---------- ##
## ls_files.  ##
## ---------- ##

# ls_files_in_dir DIR GLOBBING-PATTERNS
# -------------------------------------
# The files in DIR that match the GLOBBING-PATTERNS.
#
# The GLOBBING-PATTERNS are put in single quotes to avoid being
# caught by the shell.
ls_files_in_dir =							   \
  $(or									   \
    $(shell $(build_aux_dir)/bin/ls-files -s $(1) $(patsubst %,'%',$(2))), \
    $(error ls-files in $(1) returned nothing for: $(2)))

# ls_files GLOBBING-PATTERNS
# --------------------------
# The files in $(srcdir) that match the GLOBBING-PATTERNS.
ls_files =					\
  $(call ls_files_in_dir,$(srcdir),$(1))

EXTRA_DIST += $(build_aux_dir)/bin/ls-files




# make_path REPLACEMENT-PATTERN, DIR...
# -------------------------------------
# Apply REPLACEMENT-PATTERN to each DIR, and join the result with colons.
# For instance:
# (call make_path,foo/%/bar,a b c)
#  => foo/a/bar:foo/b/bar:foo/c/bar
build_aux_empty :=
build_aux_space := $(build_aux_empty) $(build_aux_empty)
make_path =					\
  $(subst $(build_aux_space),:,$(patsubst %,$(1),$(2)))



# Sometimes it is really convenient to see the output of the
# preprocessor.  But it's a pain to run the command by hand.
SUFFIXES += .i .ii
.c.i:
	$(COMPILE) -E -o $@ $<

.cc.ii:
	$(CXXCOMPILE) -E -o $@ $<

CLEANFILES += *.i *.ii

# Sometimes, the remaining # lines are a nuisance: the error is not
# reported against the preprocessed file.  And then we also want to
# compile these files, that's why I used extensions that can be
# directly compiled (*.c and *.cc).
SUFFIXES += .i.c .ii.cc
.i.i.c:
	grep -v '#' $< | tr -s '\n' '\n' >$@.tmp
	mv $@.tmp $@

.ii.ii.cc:
	grep -v '#' $< | tr -s '\n' '\n' >$@.tmp
	mv $@.tmp $@

CLEANFILES += *.i.c *.ii.cc

INSTALL_DATA_HOOKS =
install-data-hook: $(INSTALL_DATA_HOOKS)

INSTALL_EXEC_HOOKS =
install-exec-hook: $(INSTALL_EXEC_HOOKS)

UNINSTALL_HOOKS =
uninstall-hook: $(UNINSTALL_HOOKS)


## -------------------- ##
## Semaphores on OS X.  ##
## -------------------- ##

.PHONY check-TESTS: clean-semaphores
clean-semaphores:
	-$(build_aux_dir)/bin/semaphore-clean.sh

# An included Makefile that does not exist.  Regenerate the Makefile.
# Chances are that the missing file is no longer needed.  But if it is
# and cannot be regenerated, then we might loop...  FIXME: find a
# means to stop this.
%.mk:
	@if test ! -f $@; then						\
	  echo >&2 "Makefile:warning: "					\
	           "*** missing makefile, trying to proceed: $@" &&	\
	  echo "cd $(top_srcdir) && $(AUTOMAKE) --foreign" &&		\
	  ($(am__cd) $(top_srcdir) && $(AUTOMAKE) --foreign) &&		\
	  echo "cd $(top_builddir) &&  ./config.status --recheck" &&	\
	  ($(am__cd) $(top_builddir) && ./config.status --recheck)	\
	fi
