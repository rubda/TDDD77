BUILD_DIR = obj
INCLUDE_DIR = include
SRC_DIR = src

CC = gcc
CFLAGS = -Wall -pedantic --std=c99

.PHONY: all
all: $(BUILD_DIR)/libmatrix.a $(BUILD_DIR)/tests/test_int

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

# test_int

.PHONY: test_int
test_int: $(BUILD_DIR)/tests/test_int 

$(BUILD_DIR)/tests/test_int: $(BUILD_DIR)/libmatrix.a $(BUILD_DIR)/tests/test_int.o
	$(CC) $(CFLAGS) $^ -o $@ -L $(BUILD_DIR) -lmatrix
	chmod 755 $@
