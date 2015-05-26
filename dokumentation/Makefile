BUILDDIR = build

ASM_DIR = active-set-method
ASM_NAME = active-set-method

ARK_DIR = arkitekturdokument
ARK_NAME = arkitekturdokument

ANV_DIR = anvandarhandledning
ANV_NAME = anvandarhandledning

KRAV_DIR = kravspecifikation
KRAV_NAME = kravspecifikation

KVAL_DIR = kvalitetsplan
KVAL_NAME = kvalitetsplan

PROJ_DIR = projektplan
PROJ_NAME = projektplan

TEST_DIR = testplan
TEST_NAME = testplan

TEK_DIR = tekniskdokumentation
TEK_NAME = tekniskdokumentation

TR1_DIR = testrapport1
TR1_NAME = testrapport

TR2_DIR = testrapport2
TR2_NAME = testrapport2

UT1_DIR = utvarderingiteration1
UT1_NAME = utvarderingiteration1

UT2_DIR = utvarderingiteration2
UT2_NAME = utvarderingiteration2

PDFS =  $(ASM_DIR)/$(BUILDDIR)/$(ASM_NAME).pdf
PDFS += $(ARK_DIR)/$(BUILDDIR)/$(ARK_NAME).pdf
PDFS += $(ANV_DIR)/$(BUILDDIR)/$(ANV_NAME).pdf
PDFS += $(KRAV_DIR)/$(BUILDDIR)/$(KRAV_NAME).pdf
PDFS += $(KVAL_DIR)/$(BUILDDIR)/$(KVAL_NAME).pdf
PDFS += $(PROJ_DIR)/$(BUILDDIR)/$(PROJ_NAME).pdf
PDFS += $(TEST_DIR)/$(BUILDDIR)/$(TEST_NAME).pdf
PDFS += $(TEK_DIR)/$(BUILDDIR)/$(TEK_NAME).pdf
PDFS += $(TR1_DIR)/$(BUILDDIR)/$(TR1_NAME).pdf
PDFS += $(TR2_DIR)/$(BUILDDIR)/$(TR2_NAME).pdf
PDFS += $(UT1_DIR)/$(BUILDDIR)/$(UT1_NAME).pdf
PDFS += $(UT2_DIR)/$(BUILDDIR)/$(UT2_NAME).pdf

.PHONY: all
all: $(PDFS) kandidatrapport

.PHONY: kandidatrapport
kandidatrapport:
	cd kandidatrapport ; make all

$(ASM_DIR)/$(BUILDDIR)/$(ASM_NAME).pdf: $(ASM_DIR)/$(ASM_NAME).tex $(wildcard $(ASM_DIR)/tex/*) $(wildcard $(ASM_DIR)/grafik/*)
$(ARK_DIR)/$(BUILDDIR)/$(ARK_NAME).pdf: $(ARK_DIR)/$(ARK_NAME).tex $(wildcard $(ARK_DIR)/tex/*) $(wildcard $(ARK_DIR)/bilder/*)
$(ANV_DIR)/$(BUILDDIR)/$(ANV_NAME).pdf: $(ANV_DIR)/$(ANV_NAME).tex $(wildcard $(ANV_DIR)/tex/*) $(wildcard $(ANV_DIR)/bilder/*)
$(KRAV_DIR)/$(BUILDDIR)/$(KRAV_NAME).pdf: $(KRAV_DIR)/$(KRAV_NAME).tex $(wildcard $(KRAV_DIR)/tex/*)
$(KVAL_DIR)/$(BUILDDIR)/$(KVAL_NAME).pdf: $(KVAL_DIR)/$(KVAL_NAME).tex $(wildcard $(KVAL_DIR)/tex/*)
$(PROJ_DIR)/$(BUILDDIR)/$(PROJ_NAME).pdf: $(PROJ_DIR)/$(PROJ_NAME).tex $(wildcard $(PROJ_DIR)/tex/*) $(wildcard $(PROJ_DIR)/grafik/*)
$(TEST_DIR)/$(BUILDDIR)/$(TEST_NAME).pdf: $(TEST_DIR)/$(TEST_NAME).tex $(wildcard $(TEST_DIR)/tex/*)
$(TEK_DIR)/$(BUILDDIR)/$(TEK_NAME).pdf: $(TEK_DIR)/$(TEK_NAME).tex $(wildcard $(TEK_DIR)/tex/*) $(wildcard $(ANV_DIR)/bilder/*)
$(TR1_DIR)/$(BUILDDIR)/$(TR1_NAME).pdf: $(TR1_DIR)/$(TR1_NAME).tex $(wildcard $(TR1_DIR)/tex/*)
$(TR2_DIR)/$(BUILDDIR)/$(TR2_NAME).pdf: $(TR2_DIR)/$(TR2_NAME).tex $(wildcard $(TR2_DIR)/tex/*) $(wildcard $(TR2_DIR)/grafik/*)
$(UT1_DIR)/$(BUILDDIR)/$(UT1_NAME).pdf: $(UT1_DIR)/$(UT1_NAME).tex $(wildcard $(UT1_DIR)/tex/*)
$(UT2_DIR)/$(BUILDDIR)/$(UT2_NAME).pdf: $(UT2_DIR)/$(UT2_NAME).tex $(wildcard $(UT2_DIR)/tex/*)

$(PDFS):
	cd $(dir $<) ; mkdir -p $(BUILDDIR)
	cd $(dir $<) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) 1>/dev/null
	cd $(dir $<) ; pdflatex -halt-on-error -output-directory=$(BUILDDIR) $(notdir $<) 1>/dev/null

.PHONY: clean
clean:
	rm -rf $(dir $(PDFS))

