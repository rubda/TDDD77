SRC_DIR = src
INCLUDE_DIR = include

LIBMATRIX_DIR = ../matrixlibrary/src
LIBMATRIX_INCLUDE = ../matrixlibrary/include
LIBMATRIX = $(LIBMATRIX_DIR)/libmatrix.a

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
	$(CC) $(CFLAGS) $^ -o $@ -I $(LIBMATRIX_INCLUDE) -I $(INCLUDE_DIR) -L $(LIBMATRIX_DIR) -lmatrix

.PHONY: $(LIBMATRIX)
$(LIBMATRIX):
	cd ../matrixlibrary ; $(MAKE) src/libmatrix.a

.PHONY: clean
clean:
	rm -f $(TEST_BINS)
	rm -f $(OBJS)
