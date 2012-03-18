# Rules to convert several common figure formats to formats well
# supported by pdflatex (png, jpg, pdf).

CONVERT ?= convert
DIA ?= dia
DOT ?= dot
ENSURE_TARGET_DIR ?= mkdir -p $(dir $@) || true
EPSTOPDF ?= epstopdf
FDP ?= fdp
FIG2DEV ?= fig2dev
GNUPLOT ?= gnuplot
PDFCROP ?= pdfcrop
INKSCAPE ?= inkscape

CONVERT_TO_PDF =				\
  $(AM_V_GEN)					\
  { $(ENSURE_TARGET_DIR) ; } &&			\
  $(CONVERT) $< pdf:$@.tmp &&			\
  mv $@.tmp $@

# Formats to convert to PDF.
SHARE_EXTS_TO_PDF = \
  dot fig fdp gif id tif tiff pbm pgm ppm pdffig plt svg $(EXTS_TO_PDF)
# Formats to convert to PNG.
SHARE_EXTS_TO_PNG = dia $(EXTS_TO_PNG)

# FILES
# convert_ext SRC-EXT, DST-EXT, FILEs
# -----------------------------------
# Return the sorted list of $(FILES) with SRC-EXT as extension,
# changing to DST-EXT.
share_convert_ext =					\
   $(sort						\
          $(patsubst %.$(1),%.$(2),			\
                     $(filter %.$(1),$(3))))


# FILES
# convert_exts SRC-EXTS, DST-EXT, FILES
# -------------------------------------
# Map all the extensions in SRC-EXTS to DST-EXT of the $(FILES) list.
share_convert_exts =					\
  $(foreach ext,					\
            $(1),					\
            $(call share_convert_ext,$(ext),$(2),$(3)))


# FILES
# share_convert_to_pdf FILES
# --------------------------
# Choose the most appropriate format for PDFLaTeX for the FILES.
# Beware that unknown formats are left out.
share_convert_to_pdf =						\
    $(call share_convert_exts,$(SHARE_EXTS_TO_PDF),pdf,$(1))	\
    $(call share_convert_exts,$(SHARE_EXTS_TO_PNG),png,$(1))


# There seems to be more bugs in dia -> fig -> pdf than dia -> png.
%.png: %.dia
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(DIA) -n -e $@.tmp -t png $<
	$(AM_V_at)mv $@.tmp $@

%.png: %.fdp
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(FDP) -Tpng $< -o $@.tmp
	$(AM_V_at)mv $@.tmp $@


## This does not work properly, especially when the output is bigger
## than A4, in which case ps2epsi crops.
##
## %.eps: %.dot
## 	dot -Gpage=595.842 -Tps2 $< -o $*.ps
## # This line: [ /CropBox [36 36 97 89] /PAGES pdfmark
## # breaks the ps2pdf output.
## 	sed -i '/CropBox/d' $*.ps
## 	ps2epsi $*.ps $@

%.fig: %.dot
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(DOT) -Tfig $< -o $@.tmp
	$(AM_V_at)mv $@.tmp $@

%.pdf: %.dot
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
# Some versions of dot don't support PDF output.
	$(AM_V_at)					\
	if $(DOT) -Tpdf </dev/null 2>/dev/null; then	\
	  $(DOT) -Tpdf $< -o $@.t1 &&			\
	  $(PDFCROP) $(if $(V), --verbose, >/dev/null)	\
	    $@.t1 $@.tmp;				\
	else						\
	  $(DOT) -Tfig $< -o $@.t1 &&			\
	  $(FIG2DEV) -Lpdf -p dummy $@.t1 $@.tmp;	\
	fi
	$(AM_V_at)rm $@.t1
	$(AM_V_at)mv $@.tmp $@

%.fig: %.fdp
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(FDP) -Tfig $< -o $@.tmp
## There is a bug in fdp's fig output, see Debian bug 373005.
	$(AM_V_at)perl -0777 -pi -e 's/^2 3 0.*\n.*\n//m' $@.tmp
	$(AM_V_at)mv $@.tmp $@

%.pdf: %.fig
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(FIG2DEV) -Lpdf -p dummy $< $@.tmp
	$(AM_V_at)mv $@.tmp $@

%.eps: %.fig
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(FIG2DEV) -Leps -p dummy $< $@.tmp
	$(AM_V_at)mv $@.tmp $@


## pdftex/pdftex_t combined XFig pictures.
##
## To avoid problems, the PDF file should end with .pdf, not .pdftex
## as suggested in xfig documentation.  And to avoid ambiguity on
## *.fig -> *.pdf, let's use *.pdffig as input extension instead of
## *.fig.
##
## A single Make rule with two commands because it simplifies the use:
## depend on one file, not two.  Generate the PDF last, since it bears
## the dependency.
%.pdf %.eps %.pdftex_t: %.pdffig
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	-rm -f $*.{pdf,pdftex_t}
	$(AM_V_at)$(FIG2DEV) -Lpdftex_t -p $*.pdf $< $*.pdftex_t
# Some versions of fig2dev produce code for epsfig instead of graphicx.
# Do not force the extension to PDF.
	$(AM_V_at)						\
	perl -pi -e 's/\\epsfig\{file=/\\includegraphics{/;'	\
		 -e 's/(\\includegraphics\{.*)\.pdf/$$1/;'	\
	  $*.pdftex_t
	$(AM_V_at)$(FIG2DEV) -Lpdftex -p dummy $< $*.pdf
	$(AM_V_at)$(FIG2DEV) -Lpstex -p dummy $< $*.eps


## pdftex/pdftex_t combined GNU Plot pictures.
##
## The GNU Plot file needs not set the output file name, nor the terminal:
## set are properly set by default.
##
## A single Make rule with two commands because it simplifies the use:
## depend on one file, not two.
%.pdftex_t %.pdf: %.plt
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
# gnuplot creates an output, even if it failed.  Remove files from a
# previous run that might have us think it went ok.
	$(AM_V_at)rm -f $*.{tex,eps,pdf,pdftex_t}
# Put the output first, before the plotting command, and before
# requests from the user.  Set the terminal too there for the same
# reasons.
	$(AM_V_at){				\
	  echo 'set output "$*.tex"' &&		\
	  echo 'set terminal epslatex color' &&	\
	  cat $<;				\
	} > $*.plt.tmp
	$(AM_V_at)LC_ALL=C $(GNUPLOT) $*.plt.tmp
	$(AM_V_at)mv $*.tex $*.pdftex_t
	$(AM_V_at)$(EPSTOPDF) $*.eps -o $*.pdf
# Remove the EPS file.  See
# http://lists.gnu.org/archive/html/bug-make/2011-05/msg00001.html
	$(AM_V_at)rm $*.plt.tmp $*.eps

%.pdf: %.gif
	$(CONVERT_TO_PDF)
%.pdf: %.id
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(EPSTOPDF) $< -o pdf:$@.tmp
	$(AM_V_at)mv $@.tmp $@

#%.pdf: %.eps
#	$(ENSURE_TARGET_DIR)
#	$(EPSTOPDF) $< -o $@.tmp
#	mv $@.tmp $@

%.pdf: %.jpg
	$(CONVERT_TO_PDF)
%.pdf: %.png
	$(CONVERT_TO_PDF)
%.pdf: %.tif
	$(CONVERT_TO_PDF)
%.pdf: %.tiff
	$(CONVERT_TO_PDF)
%.pdf: %.pbm
	$(CONVERT_TO_PDF)
%.pdf: %.pgm
	$(CONVERT_TO_PDF)
%.pdf: %.ppm
	$(CONVERT_TO_PDF)

%.pdf: %.svg
	$(AM_V_GEN)$(ENSURE_TARGET_DIR)
	$(AM_V_at)$(INKSCAPE) --export-pdf=$@.tmp $<
	$(AM_V_at)mv $@.tmp $@
