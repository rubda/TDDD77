.PHONY: all
all: active-set-method/active-set-method.pdf arkitekturdokument/arkitekturdokument.pdf förstudie/forstudie.pdf kravspecifikation/kravspecifikation.pdf kvalitetsplan/kvalitetsplan.pdf projektplan/projektplan.pdf testplan/testplan.pdf

.PHONY: clean
clean:
	rm -f active-set-method/active-set-method.pdf
	rm -f arkitekturdokument/arkitekturdokument.pdf
	rm -f förstudie/forstudie.pdf
	rm -f kravspecifikation/kravspecifikation.pdf
	rm -f kvalitetsplan/kvalitetsplan.pdf
	rm -f projektplan/projektplan.pdf
	rm -f testplan/testplan.pdf

active-set-method/active-set-method.pdf: active-set-method/active-set-method.tex
	cd active-set-method ; pdflatex active-set-method.tex 1>/dev/null

arkitekturdokument/arkitekturdokument.pdf: arkitekturdokument/arkitekturdokument.tex
	cd arkitekturdokument ; pdflatex arkitekturdokument.tex 1>/dev/null

förstudie/forstudie.pdf: förstudie/forstudie.tex
	cd förstudie ; pdflatex forstudie.tex 1>/dev/null

kravspecifikation/kravspecifikation.pdf: kravspecifikation/kravspecifikation.tex
	cd kravspecifikation ; pdflatex kravspecifikation.tex 1>/dev/null

kvalitetsplan/kvalitetsplan.pdf: kvalitetsplan/kvalitetsplan.tex
	cd kvalitetsplan ; pdflatex kvalitetsplan.tex 1>/dev/null

projektplan/projektplan.pdf: projektplan/projektplan.tex
	cd projektplan ; pdflatex projektplan.tex 1>/dev/null

testplan/testplan.pdf: testplan/testplan.tex
	cd testplan ; pdflatex testplan.tex 1>/dev/null
