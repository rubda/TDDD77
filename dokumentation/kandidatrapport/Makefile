BUILDDIR = build

KAND_NAME = kandidatrapport

ADAM_NAME = kandidatrapport-adam
ADAM_DIR = adam-tex

DENNIS_NAME = kandidatrapport-dennis
DENNIS_DIR = dennis-tex

MARTIN_NAME = kandidatrapport-martin
MARTIN_DIR = martin-tex

RUBEN_NAME = kandidatrapport-ruben
RUBEN_DIR = ruben-tex

YNGVE_NAME = kandidatrapport-yngve
YNGVE_DIR = yngve-tex

NAMES = $(KAND_NAME) $(ADAM_NAME) $(DENNIS_NAME) $(MARTIN_NAME) $(RUBEN_NAME) $(YNGVE_NAME)

PDFS = $(addprefix $(BUILDDIR)/, $(addsuffix .pdf,$(NAMES)))

ifndef OUTPUT
OUT = 1>/dev/null
else
OUT = 
endif


.PHONY: all
all: $(PDFS)

$(BUILDDIR)/$(KAND_NAME).pdf: $(KAND_NAME).tex $(wildcard tex/*) $(wildcard grafik/*)
$(BUILDDIR)/$(ADAM_NAME).pdf: $(ADAM_NAME).tex $(wildcard $(ADAM_DIR)/*)
$(BUILDDIR)/$(DENNIS_NAME).pdf: $(DENNIS_NAME).tex $(wildcard $(DENNIS_DIR)/*)
$(BUILDDIR)/$(MARTIN_NAME).pdf: $(MARTIN_NAME).tex $(wildcard $(MARTIN_DIR)/*)
$(BUILDDIR)/$(RUBEN_NAME).pdf: $(RUBEN_NAME).tex $(wildcard $(RUBEN_DIR)/*) $(wildcard $(RUBEN_DIR)/graphic/*)
$(BUILDDIR)/$(YNGVE_NAME).pdf: $(YNGVE_NAME).tex $(wildcard $(YNGVE_DIR)/*) $(wildcard $(YNGVE_DIR)/grafik/*)

$(PDFS):
	cd $(dir $<) ; mkdir -p $(BUILDDIR)
	cd $(dir $<) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) $(OUT)
#	cd $(dir $<) ; bibtex $(BUILDDIR)/$(basename $(notdir $<)).aux
	cd $(dir $<) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) $(OUT)
	cd $(dir $<) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) $(OUT)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

