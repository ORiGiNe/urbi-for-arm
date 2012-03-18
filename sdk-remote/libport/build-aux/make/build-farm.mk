# This is a modified version of Automake 1.10.2's distcheck.  The
# differences are:
#
# - we don't run "make dvi", we're not interested in it.
#
# - we don't run "make check", since the maintainers already do
#   constantly.
#
# - we don't run "make installcheck", as the build-farm already does
#   before running distcheck.
#
# - we don't run DISTCHECK installs, it's too costly yet, and our
#   semantics is slightly different.  Will be addressed eventually.
#
# - we disable the uninstallcheck because we are not ready yet.
#
# - we introduce DISTCHECK_INSTALLCHECK_FLAGS which defaults to
#   passing VERBOSE to 1.
.PHONY: distcheck-buildfarm
distcheck-buildfarm: mydistcheck

.PHONY: mydistcheck
mydistcheck: dist
	case '$(DIST_ARCHIVES)' in					 \
	*.tar.gz*)							 \
	  GZIP=$(GZIP_ENV) gunzip -c $(distdir).tar.gz | $(am__untar) ;; \
	*.tar.bz2*)							 \
	  bunzip2 -c $(distdir).tar.bz2 | $(am__untar) ;;		 \
	*.tar.lzma*)							 \
	  unlzma -c $(distdir).tar.lzma | $(am__untar) ;;		 \
	*.tar.Z*)							 \
	  uncompress -c $(distdir).tar.Z | $(am__untar) ;;		 \
	*.shar.gz*)							 \
	  GZIP=$(GZIP_ENV) gunzip -c $(distdir).shar.gz | unshar ;;	 \
	*.zip*)								 \
	  unzip $(distdir).zip ;;					 \
	esac
	chmod -R a-w $(distdir); chmod a+w $(distdir)
	mkdir $(distdir)/_build
	mkdir $(distdir)/_inst
	chmod a-w $(distdir)
	dc_install_base=`$(am__cd) $(distdir)/_inst && pwd | sed -e 's,^[^:\\/]:[\\/],/,'` \
	  && dc_destdir="$${TMPDIR-/tmp}/am-dc-$$$$/" \
	  && cd $(distdir)/_build \
	  && ../configure --srcdir=.. --prefix="$$dc_install_base" \
	    $(DISTCHECK_CONFIGURE_FLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) \
##	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
##	  && $(MAKE) $(AM_MAKEFLAGS) check
	  && $(MAKE) $(AM_MAKEFLAGS) install \
##	  && $(MAKE) $(AM_MAKEFLAGS) installcheck $(DISTCHECK_INSTALLCHECK_FLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && $(MAKE) $(AM_MAKEFLAGS) distuninstallcheck_dir="$$dc_install_base" \
	        mydistuninstallcheck \
	  && chmod -R a-w "$$dc_install_base" \
##	  && ({ \
##	       (cd ../.. && umask 077 && mkdir "$$dc_destdir") \
##	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" install \
##	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" uninstall \
##	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" \
##	            distuninstallcheck_dir="$$dc_destdir" mydistuninstallcheck; \
##	      } || { rm -rf "$$dc_destdir"; exit 1; }) \
	  && rm -rf "$$dc_destdir" \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && rm -rf $(DIST_ARCHIVES) \
	  && $(MAKE) $(AM_MAKEFLAGS) mydistcleancheck
	$(am__remove_distdir)
	@(echo "$(distdir) archives ready for distribution: "; \
	  list='$(DIST_ARCHIVES)'; for i in $$list; do echo $$i; done) | \
	  sed -e 1h -e 1s/./=/g -e 1p -e 1x -e '$$p' -e '$$x'

# Do not try to render colors in the output.
#
# Instrument the tests: installcheck (the only tests run by
# distcheck-buildfarm) the best moment to do that since we no longer
# run wrappers, but really the programs themselves.  But instrument
# only part of the time, Valgrind is *very* slow, and we cannot afford
# to wait for it to have run all the tests.
#
# Report the logs, as otherwise it just display a big fat silent
# failure.
DISTCHECK_INSTALLCHECK_FLAGS ?=			\
  AM_COLOR_TESTS=no				\
  INSTRUMENTATION=0.02				\
  VERBOSE=1

# Same as Automake's distuninstallcheck and mydistcleancheck, but with
# exit 0 instead of exit 1: these are useful warnings, but don't make
# them error (yet): we are not ready.
.PHONY: mydistcleancheck
mydistuninstallcheck:
	@$(am__cd) '$(distuninstallcheck_dir)' \
	&& test `$(distuninstallcheck_listfiles) | wc -l` -le 1 \
	   || { echo "ERROR: files left after uninstall:" ; \
	        if test -n "$(DESTDIR)"; then \
	          echo "  (check DESTDIR support)"; \
	        fi ; \
	        $(distuninstallcheck_listfiles) ; \
	        exit 0; } >&2

.PHONY: mydistcleancheck
mydistcleancheck:
## FIXME: something goes wrong, and clean fails in Urbi SDK 2.0.2.
## mydistcleancheck: distclean
## 	@if test '$(srcdir)' = . ; then \
## 	  echo "ERROR: distcleancheck can only run from a VPATH build" ; \
## 	  exit 1 ; \
## 	fi
## 	@test `$(distcleancheck_listfiles) | wc -l` -eq 0 \
## 	  || { echo "ERROR: files left in build directory after distclean:" ; \
## 	       $(distcleancheck_listfiles) ; \
## 	       exit 0; } >&2

# Remove the dists and distdirs that we made.
#
# Don't remove the current one, $(distdir).
#
# Don't use xargs when there might not be arguments, on GNU/Linux it
# runs the command anyway, without arguments, which results in useless
# noise on stderr.
.PHONY: dists-clean
dists-clean:
	for i in $(PACKAGE)-*;					\
	do							\
	  case $$i$$(test ! -d $$i || echo /) in		\
	    ($(distdir)*);;					\
	    (*.tar.* | *.zip) remove+=" $$i";;			\
	    (*/) test ! -f $$i/configure || remove+=" $$i";;	\
	  esac;							\
	done;							\
	echo "Removing former distributions...";		\
	for i in $$remove;					\
	do							\
	  echo "... $$i";					\
	  find $$i -type d ! -perm -200 -exec chmod u+w {} ';';	\
	  rm -rf $$i;						\
	done;							\
	echo "... done"

# Always clean beforehand, because since our VERSION changes
# frequently, we leave bazillions of distdirs and dists.
distcheck-buildfarm: dists-clean
