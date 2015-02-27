BUILD_DIR = build
SRC_DIR = src
DOCS_DIR = dokumentation
LIBS = -lgsl -lgslcblas
CC = gcc
CFLAGS = -Wall -pedantic --std=c99

.PHONY: all
all: test

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	cd $(DOCS_DIR) ; make clean

.PHONY: docs
docs:
	cd $(DOCS_DIR) ; make all
