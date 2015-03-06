BUILD_DIR = obj
SRC_DIR = src

LIBMATRIX_DIR = ../matrixlibrary/obj
LIBMATRIX_INCLUDE = ../matrixlibrary/include
LIBMATRIX = $(LIBMATRIX_DIR)/libmatrix.a

CC = gcc
CFLAGS = -Wall -pedantic --std=c99

TESTS = $(basename $(notdir $(wildcard $(SRC_DIR)/tests/*.c)))

.PHONY: all
all: $(addprefix $(BUILD_DIR)/tests/, $(TESTS))

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(SRC_DIR)/*.h) $(LIBMATRIX)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -L $(LIBMATRIX_DIR) -l matrix -I $(LIBMATRIX_INCLUDE) -Isrc


# Matrixlibrary

.PHONY: $(LIBMATRIX)
$(LIBMATRIX):
	cd ../matrixlibrary ; make libmatrix


# Tests

.PHONY: tests
tests: $(addprefix $(BUILD_DIR)/tests/, $(TESTS))

$(BUILD_DIR)/tests/%: $(BUILD_DIR)/tests/%.o $(BUILD_DIR)/work_set.o $(BUILD_DIR)/find_lagrange.o
	$(CC) $(CFLAGS) $^ -o $@ -L $(LIBMATRIX_DIR) -l matrix
	chmod 755 $@

