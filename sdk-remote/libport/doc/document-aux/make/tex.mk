# Make sure we don't become promoted as default target.
all:
.PHONY: all

share_style_dir = $(share_dir)/styles
share_bib_dir = $(share_dir)/bib
ChangeLog ?= ChangeLog

TEXI2DVI = $(share_bin_dir)/texi2dvi
TEXI2DVIFLAGS = --tidy --build-dir=tmp.t2d --batch
# Find the style files, the bibliography files, and the figures.
# Arguably, we should always use qualified names, but this is annoying
# (at least for style files, it is acceptable for bib files).  We do
# it nevertheless for figures, because experience has already bitten
# me (AD).  So use \includegraphics{figs/lrde-big} instead of
# {lrde-big}.
TEXI2DVIFLAGS += -I $(share_style_dir) -I $(share_bib_dir) -I $(share_dir)

TEXI2PDF = $(TEXI2DVI) --pdf
TEXI2PDFFLAGS = $(TEXI2DVIFLAGS)

TEXI2HTML = $(TEXI2DVI) --html
TEXI2HTMLFLAGS = $(TEXI2DVIFLAGS)

TEXI2TEXT = $(TEXI2DVI) --text
TEXI2TEXTFLAGS = $(TEXI2DVIFLAGS)

TEXI2INFO = $(TEXI2DVI) --info
TEXI2INFOFLAGS = $(TEXI2DVIFLAGS)

share_tex_dependencies = \
$(STYLES) \
$(wildcard $(share_style_dir)/* $(share_bib_dir)/*)


## ------- ##
## *.tex.  ##
## ------- ##

%.dvi: %.tex $(share_tex_dependencies)
	$(TEXI2DVI) $(TEXI2DVIFLAGS) -o $@ $<

%.html: %.tex $(share_tex_dependencies)
	$(TEXI2HTML) $(TEXI2HTMLFLAGS) -o $@ $<

%.info: %.tex $(share_tex_dependencies)
	$(TEXI2INFO) $(TEXI2INFOFLAGS) -o $@ $<

%.pdf: %.tex $(share_tex_dependencies)
	$(TEXI2PDF) $(TEXI2PDFFLAGS) -o $@ $<

%.txt: %.tex $(share_tex_dependencies)
	$(TEXI2TEXT) $(TEXI2TEXTFLAGS) -o $@ $<


## ------- ##
## *.ltx.  ##
## ------- ##

%.dvi: %.ltx $(share_tex_dependencies)
	$(TEXI2DVI) $(TEXI2DVIFLAGS) -o $@ $<

%.info: %.ltx $(share_tex_dependencies)
	$(TEXI2INFO) $(TEXI2INFOFLAGS) -o $@ $<

%.html: %.ltx $(share_tex_dependencies)
	$(TEXI2HTML) $(TEXI2HTMLFLAGS) -o $@ $<

%.pdf: %.ltx $(share_tex_dependencies)
	$(TEXI2PDF) $(TEXI2PDFFLAGS) -o $@ $<

%.txt: %.ltx $(share_tex_dependencies)
	$(TEXI2TEXT) $(TEXI2TEXTFLAGS) -o $@ $<



## --------- ##
## rev.sty.  ##
## --------- ##

rev.sty: $(ChangeLog)
	if svn --version >/dev/null 2>&1; then			\
	  $(share_bin_dir)/generate-rev-sty $< >$@;		\
	elif test -f $@; then					\
	  touch $@;						\
	else							\
	  echo '@newcommand{@SvnRev}{}' | tr '@' '\\' >$@;	\
	fi

rev.tex: rev.sty
	@echo >&2 "******* rev.tex is deprecated, using rev.sty instead"
	{								   \
	  echo "\PackageWarning{rev}";					   \
	  echo "        {rev.tex is deprecated, using rev.sty instead}{}"; \
	  cat rev.sty;							   \
	} >$@

CLEANFILES += rev.{sty,tex}

tex-mostlyclean:
	rm -rf tmp.t2d
.PHONY: tex-mostlyclean
# mostlyclean-local is an Automake special target.
mostlyclean-local: tex-mostlyclean
.PHONY: mostlyclean-local
