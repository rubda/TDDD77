PACKNAME = quadopt_v1.0

.PHONY: all
all: docs libmatrix quadopt

.PHONY: clean
clean:
	cd dokumentation ; make clean
	cd matrixlibrary ; make clean
	cd quadopt ; make clean
	rm -rf $(PACKNAME)
	rm -f $(PACKNAME).zip

.PHONY: docs
docs:
	cd dokumentation ; make all

.PHONY: libmatrix
libmatrix:
	cd matrixlibrary ; make all

.PHONY: quadopt
quadopt:
	cd quadopt ; make all

.PHONY: test
test:
	cd matrixlibrary ; make test
	cd quadopt ; make test

.PHONY: matlab
matlab:
	cd matlab; make


.PHONY: package
package: quadopt libmatrix docs
	mkdir -p $(PACKNAME)
	cp QuadOptGUI/package_makefile.mk $(PACKNAME)/Makefile
	cp QuadOptGUI/CustomText.py $(PACKNAME)/CustomText.py
	cp QuadOptGUI/GUI.py $(PACKNAME)/GUI.py
	cp QuadOptGUI/parser.py $(PACKNAME)/parser.py
	mkdir -p $(PACKNAME)/quadopt
	mkdir -p $(PACKNAME)/quadopt/src
	mkdir -p $(PACKNAME)/quadopt/include
	cp quadopt/include/*.h $(PACKNAME)/quadopt/include
	cp quadopt/src/*.c $(PACKNAME)/quadopt/src
	mkdir -p $(PACKNAME)/matrixlibrary
	mkdir -p $(PACKNAME)/matrixlibrary/obj
	mkdir -p $(PACKNAME)/matrixlibrary/include
	cp matrixlibrary/obj/libmatrix.a $(PACKNAME)/matrixlibrary/obj
	cp matrixlibrary/include/*.h $(PACKNAME)/matrixlibrary/include
	mkdir -p $(PACKNAME)/matlab
	cp matlab/build.m $(PACKNAME)/matlab
	cp matlab/quadopt.c $(PACKNAME)/matlab
	mkdir -p $(PACKNAME)/dokumentation
	cp dokumentation/anvandarhandledning/build/anvandarhandledning.pdf $(PACKNAME)/dokumentation/
	cp dokumentation/tekniskdokumentation/build/tekniskdokumentation.pdf $(PACKNAME)/dokumentation/
	zip -r $(PACKNAME).zip $(PACKNAME)
	rm -rf $(PACKNAME)
