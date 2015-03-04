BUILD_DIR = obj
INCLUDE_DIR = include
SRC_DIR = src

CC = gcc
CFLAGS = -Wall -pedantic --std=c99

.PHONY: all
all: $(addprefix $(BUILD_DIR)/, test_int_2 test_int libmatrix.a)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h Makefile
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@ 

# Rule for making the library

.PHONY: libmatrix
libmatrix: $(BUILD_DIR)/libmatrix.a

$(BUILD_DIR)/libmatrix.a: $(BUILD_DIR)/matLib.o
	ar rcs $@ $<
	chmod 644 $@

# test_int_2

.PHONY: test_int_2
test_int_2: $(BUILD_DIR)/test_int_2 

$(BUILD_DIR)/test_int_2: $(addprefix $(BUILD_DIR)/, libmatrix.a test_int_2.o)
	$(CC) $(CFLAGS) $^ -o $@ -L $(BUILD_DIR) -lmatrix
	chmod 755 $@

# test_int

.PHONY: test_int
test_int: $(BUILD_DIR)/test_int 

$(BUILD_DIR)/test_int: $(addprefix $(BUILD_DIR)/, libmatrix.a test_int.o)
	$(CC) $(CFLAGS) $^ -o $@ -L $(BUILD_DIR) -lmatrix
	chmod 755 $@


