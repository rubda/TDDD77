.PHONY: all
all: docs libmatrix quadopt

.PHONY: clean
clean:
	cd matrixlibrary ; make clean
	cd quadopt ; make clean

.PHONY: libmatrix
libmatrix:
	cd matrixlibrary ; make all

.PHONY: quadopt
quadopt:
	cd quadopt ; make all
