.PHONY: all
all: libmatrix quadopt

.PHONY: clean
clean:
	cd matrixlibrary ; $(MAKE) clean
	cd quadopt ; $(MAKE) clean

.PHONY: libmatrix
libmatrix:
	cd matrixlibrary ; $(MAKE)

.PHONY: quadopt
quadopt: libmatrix
	cd quadopt ; $(MAKE)
