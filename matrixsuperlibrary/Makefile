BUILD_DIR = obj
INCLUDE_DIR = include
SRC_DIR = src

LIBMATRIX_DIR = ../matrixlibrary/obj
LIBMATRIX_SRC = ../matrixlibrary/src
LIBMATRIX_INCLUDE = ../matrixlibrary/include
LIBMATRIX = $(LIBMATRIX_DIR)/libmatrix.a

CC = gcc
CFLAGS = -g -Wall -pedantic --std=c99

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(SRCS)))
LIB = $(BUILD_DIR)/libsupermatrix.a

TEST_SRCS = $(wildcard $(SRC_DIR)/tests/*.c)
TEST_OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(TEST_SRCS)))
TEST_BINS = $(basename $(TEST_OBJS))


.PHONY: all
all: $(LIB) test

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -I $(LIBMATRIX_INCLUDE) -c $< -o $@ 

$(BUILD_DIR)/matSuperLib.o: $(SRC_DIR)/matSuperLib.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -I $(INCLUDE_DIR) -I $(LIBMATRIX_INCLUDE) -c $< -o $@ 


# Rule for making the superlibrary

.PHONY: libsupermatrix
libsupermatrix: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^
	chmod 644 $@


# Matrixlibrary

$(LIBMATRIX): $(wildcard $(LIBMATRIX_SRC)/*.c) $(wildcard $(LIBMATRIX_INCLUDE)/*.h)
	cd ../matrixlibrary ; $(MAKE) libmatrix


# Tests

.PHONY: test
test: $(TEST_BINS)
	@echo Running tests...
	@$(foreach test_bin, $(TEST_BINS), echo Running: $(test_bin) && $(test_bin) &&) true

$(TEST_BINS): $(TEST_OBJS) $(LIB) $(LIBMATRIX)
	$(CC) $@.o -o $@ -L $(BUILD_DIR) -L $(LIBMATRIX_DIR) -lsupermatrix -lmatrix -pthread
	chmod 755 $@
