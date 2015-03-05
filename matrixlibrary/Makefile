BUILD_DIR = obj
INCLUDE_DIR = include
SRC_DIR = src

CC = gcc
CFLAGS = -Wall -pedantic --std=c99

TESTS = $(basename $(notdir $(wildcard $(SRC_DIR)/tests/*.c)))

.PHONY: all
all: $(BUILD_DIR)/libmatrix.a $(addprefix $(BUILD_DIR)/tests/, $(TESTS))

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@ 


# Rule for making the library

.PHONY: libmatrix
libmatrix: $(BUILD_DIR)/libmatrix.a

$(BUILD_DIR)/libmatrix.a: $(BUILD_DIR)/matLib.o
	ar rcs $@ $<
	chmod 644 $@


# Tests

.PHONY: tests
tests: $(addprefix $(BUILD_DIR)/tests/, $(TESTS))

$(BUILD_DIR)/tests/%: $(BUILD_DIR)/tests/%.o $(BUILD_DIR)/libmatrix.a 
	$(CC) $(CFLAGS) $< -o $@ -L $(BUILD_DIR) -lmatrix
	chmod 755 $@
