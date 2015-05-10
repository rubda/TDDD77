BUILDDIR = build
OUTDIR = pdf

PDFS =  active-set-method/$(OUTDIR)/active-set-method.pdf
PDFS += arkitekturdokument/$(OUTDIR)/arkitekturdokument.pdf
PDFS += kravspecifikation/$(OUTDIR)/kravspecifikation.pdf
PDFS += kvalitetsplan/$(OUTDIR)/kvalitetsplan.pdf
PDFS += projektplan/$(OUTDIR)/projektplan.pdf
PDFS += testplan/$(OUTDIR)/testplan.pdf

.PHONY: all
all: $(PDFS)

.PHONY: clean
clean:
	rm -rf $(dir $(PDFS))
	rm -rf $(subst /$(OUTDIR)/,/$(BUILDDIR)/,$(dir $(PDFS)))

$(PDFS): $(subst /$(OUTDIR),,$(PDFS))

%.pdf: %.tex
	cd $(dir $@) ; mkdir -p $(BUILDDIR)
	cd $(dir $@) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) 1>/dev/null
	cd $(dir $@) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) 1>/dev/null
	cd $(subst /$(OUTDIR)/,,$(dir $@)) ; mkdir -p $(OUTDIR)
	cd $(dir $@) ; cp $(BUILDDIR)/$(addsuffix .pdf,$(basename $(notdir $@))) $(OUTDIR)
	cd $(dir $@) ; mv $(BUILDDIR)/$(addsuffix .pdf,$(basename $(notdir $@))) .


