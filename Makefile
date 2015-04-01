.PHONY: all
all: docs libmatrix quadopt

.PHONY: clean
clean:
	cd dokumentation ; make clean
	cd matrixlibrary ; make clean
	cd quadopt ; make clean

.PHONY: docs
docs:
	cd dokumentation ; make all

.PHONY: libmatrix
libmatrix:
	cd matrixlibrary ; make all

.PHONY: quadopt
quadopt:
	cd quadopt ; make all

.PHONY: libsupermatrix
libmatrix:
	cd matrixsuperlibrary ; make all

.PHONY: quadopt_refactor
quadopt:
	cd quadopt_refactor ; make all

.PHONY: test
test:
	cd matrixlibrary ; make test
	cd quadopt ; make test
	cd matrixsuperlibrary ; make test
	cd quadopt_refactor ; make test
