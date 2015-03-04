DOCS_DIR = dokumentation


.PHONY: all
all: docs libmatrix

.PHONY: clean
clean:
	cd $(DOCS_DIR) ; make clean
	cd matrixlibrary ; make clean

.PHONY: docs
docs:
	cd $(DOCS_DIR) ; make all

.PHONY: libmatrix
libmatrix:
	cd matrixlibrary ; make all