PDFS = active-set-method/active-set-method.pdf arkitekturdokument/arkitekturdokument.pdf kravspecifikation/kravspecifikation.pdf kvalitetsplan/kvalitetsplan.pdf projektplan/projektplan.pdf testplan/testplan.pdf

.PHONY: all
all: $(PDFS)

.PHONY: clean
clean:
	rm -f $(PDFS)

%.pdf: %.tex
	cd $(dir $@) ; pdflatex $(notdir $<) 1>/dev/null
