## Copyright (C) 2011-2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file make/bench.mk
## This file is part of build-aux.

## -------------------- ##
## malloc hook module.  ##
## -------------------- ##

BENCH_MALLOC_HOOK_SRC = $(build_aux_srcdir)/src/mhook.c
BENCH_MALLOC_HOOK = $(abs_top_builddir)/build-aux/src/mhook$(DLMODEXT)

$(BENCH_MALLOC_HOOK): $(BENCH_MALLOC_HOOK_SRC)
	$(AT_V_GEN) mkdir -p $(dir $@)
	$(AT_V_at) $(CXX) -shared $< -o $@


## ------------- ##
## bench suite.  ##
## ------------- ##

BENCH_GITDIR ?= $(top_srcdir)

BENCH_SCRIPT = $(build_aux_srcdir)/bin/bench
BENCH_NOTES =  $(build_aux_srcdir)/bin/bench-notes
BENCH_LOG =    $(build_aux_srcdir)/bin/bench-log

BENCH_LOG_FILE ?= bench-suite.log
BENCH_REMOTE ?= origin
BENCH_TITLE ?= speed #FIXME: use autotools to retrieve compilation mode.

.PHONY: $(BENCH_LOG_FILE) bench bench-store bench-push bench-buildfarm

$(BENCH_LOG_FILE): $(BENCH_MALLOC_HOOK)
	$(AM_V_GEN) $(BENCH_LOG) $(BENCH_LOGFLAGS) --flags='$(BENCHFLAGS)' \
	  -o $@.tmp $(BENCH_LOGS)
	@mv $@.tmp $@

bench-store:
	$(AM_V_GEN) $(BENCH_NOTES) $(if $(V),-v) -C $(BENCH_GITDIR) \
	  $(BENCH_TITLE) $(BENCH_LOG_FILE)

bench-run-store: bench bench-store

bench-clean:
	$(AM_V_GEN) $(BENCH_NOTES) $(if $(V),-v) -C $(BENCH_GITDIR) \
	  --discard $(BENCH_TITLE)

bench-push:
	$(AM_V_GEN) $(BENCH_NOTES) $(if $(V),-v) -C $(BENCH_GITDIR) \
	  --push --remote=$(BENCH_REMOTE) $(BENCH_TITLE)

bench-buildfarm: bench bench-notes bench-push
