BUILD_DIR = obj
SRC_DIR = src
INCLUDE_DIR = include

LIBMATRIX_DIR = ../matrixlibrary/obj
LIBMATRIX_SRC = ../matrixlibrary/src
LIBMATRIX_INCLUDE = ../matrixlibrary/include
LIBMATRIX = $(LIBMATRIX_DIR)/libmatrix.a

CC = gcc
CFLAGS = -O3 -Wall -Wextra -pedantic --std=c99

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(SRCS)))

TEST_SRCS = $(wildcard $(SRC_DIR)/tests/*.c)
TEST_OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(TEST_SRCS)))
TEST_BINS = $(basename $(TEST_OBJS))


.PHONY: all
all: $(OBJS) test


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(LIBMATRIX_INCLUDE) -I $(INCLUDE_DIR)


# Matrixlibrary

$(LIBMATRIX): $(wildcard $(LIBMATRIX_SRC)/*.c) $(wildcard $(LIBMATRIX_INCLUDE)/*.h)
	cd ../matrixlibrary ; $(MAKE) libmatrix


# Tests

.PHONY: test
test: $(TEST_BINS)
	@echo Running tests...
	@$(foreach test_bin, $(TEST_BINS), echo Running: $(test_bin) && $(test_bin) &&) true

$(TEST_BINS): $(TEST_OBJS) $(OBJS) $(LIBMATRIX)
	$(CC) $(CFLAGS) $@.o $(OBJS) -o $@ -L $(LIBMATRIX_DIR) -lmatrix -lm -pthread
	chmod 755 $@
