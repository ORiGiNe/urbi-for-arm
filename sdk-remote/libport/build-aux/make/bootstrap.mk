## Copyright (C) 2009-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file make/bench.mk
## This file is part of build-aux.

## ----------- ##
## Bootstrap.  ##
## ----------- ##

# boostrap itself touches the timestamp.  Make it a dependency on all
# instead of being part of BUILT_SOURCES, since the latter is pushed
# in CLEANFILES, which means that "make clean" would trigger a
# reconfiguration!
ALLS += $(srcdir)/bootstrap.stamp
MAINTAINERCLEANFILES += $(srcdir)/bootstrap.stamp

# Everything that should trigger a re-boostrapping.
BOOTSTRAP_sources =                             \
  $(BOOTSTRAP_CFG)                              \
  $(build_aux_dir)/bin/bootstrap                \
  $(build_aux_dir)/make/bootstrap.mk

dist_noinst_DATA +=                             \
  $(BOOTSTRAP_sources)                          \
  $(srcdir)/bootstrap.stamp

dist_noinst_SCRIPTS +=                          \
  bootstrap

$(srcdir)/bootstrap.stamp: $(BOOTSTRAP_sources)
	$(AM_V_GEN)echo '$@: $?' >$@.tmp
	$(AM_V_at)cd $(top_srcdir) && ./bootstrap
	$(AM_V_at)cat $@.tmp >>$@
	$(AM_V_at)rm -f $@.tmp
