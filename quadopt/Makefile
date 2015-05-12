SRC_DIR = src
INCLUDE_DIR = include

LIBMATRIX_DIR = ../matrixlibrary/src
LIBMATRIX_INCLUDE = ../matrixlibrary/include
LIBMATRIX_NAME = matLib
LIBMATRIX_LIB = lib$(LIBMATRIX_NAME).a
LIBMATRIX = $(LIBMATRIX_DIR)/$(LIBMATRIX_LIB)

CC = gcc
CFLAGS = -Wall -Wextra -pedantic --std=c99

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(subst .c,.o,$(SRCS))

TEST_SRCS = $(wildcard $(SRC_DIR)/tests/*.c)
TEST_BINS = $(subst .c,,$(TEST_SRCS))

.PHONY: all
all: $(OBJS) $(TEST_BINS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(LIBMATRIX_INCLUDE) -I $(INCLUDE_DIR)

$(SRC_DIR)/tests/%: $(SRC_DIR)/tests/%.c $(OBJS) $(LIBMATRIX)
	$(CC) $(CFLAGS) $^ -o $@ -I $(LIBMATRIX_INCLUDE) -I $(INCLUDE_DIR) -L $(LIBMATRIX_DIR) -l$(LIBMATRIX_NAME)

$(LIBMATRIX):
	cd ../matrixlibrary ; $(MAKE) all

.PHONY: clean
clean:
	rm -f $(TEST_BINS)
	rm -f $(OBJS)
