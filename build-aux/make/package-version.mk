## Copyright (C) 2010, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## Create the .version file.

# Ship it, so that when we are detached from a repository, we still
# have it available.  Besides, since "dist" depends on "EXTRA_DIST",
# it ensures that each time we run "make dist", ".version" is updated,
# which is what we want.
EXTRA_DIST +=					\
  $(VERSIONIFY)					\
  $(VERSIONIFY_CACHE)

all: $(VERSIONIFY_CACHE)

# Update $(VERSIONIFY_CACHE).
.PHONY: $(VERSIONIFY_CACHE)
$(VERSIONIFY_CACHE): $(VERSIONIFY)
	$(AM_V_GEN)
	$(AM_V_at)$(VERSIONIFY_RUN) --update $(VERSIONIFYFLAGS)

.PHONY: debug debug-version
debug: debug-version
debug-version:
	@echo "\$$(VERSION) = $(VERSION)"
