## Copyright (C) 2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

## \file make/html-dir.mk
## This file is part of build-aux.

## This file provides support for html_DIR, which is similar to
## html_DATA, except that it makes easier to install directories.
##
## Assign to html_DIR or EXTRA_html_DIR the top-level of the trees
## that must be installed in $(htmldir).  What's put in html_DIR is
## built at "make" time, what's in EXTRA_html_DIR might be compiled
## only at "make install" time.

html_DIR =
EXTRA_html_DIR =
## So that "make" does what "make install" wants.
noinst_DATA += $(html_DIR)
CLEANDIRS += $(html_DIR) $(EXTRA_html_DIR)

.PHONY: install-html-dir
INSTALL_DATA_HOOKS += install-html-dir

## Doxygen creates *.md5 and *.map files that serve only for
## incremental builds.  Do not propagate them, they are not needed to
## browse the documentation
## (http://osdir.com/ml/text.doxygen.general/2005-04/msg00064.html).
install-html-dir: $(html_DIR) $(EXTRA_html_DIR)
	$(mkdir_p) $(DESTDIR)$(htmldir)
	l1='$(html_DIR)';				\
	l2='$(EXTRA_html_DIR)';				\
	for d in $$l1 $$l2;				\
	do						\
	  base=$$(basename $$d);			\
	  dir=$$(dirname $$d);				\
	  tar -C $$dir					\
	    --exclude '*.md5' --exclude '*.map'		\
	    -cf - $$base				\
	    | tar -C $(DESTDIR)$(htmldir) -xf -;	\
	done
