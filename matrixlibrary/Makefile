BUILD_DIR = obj
INCLUDE_DIR = include
SRC_DIR = src

CC = gcc
CFLAGS = -g -Wall -pedantic --std=c99

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(SRCS)))
LIB = $(BUILD_DIR)/libmatrix.a

TEST_SRCS = $(wildcard $(SRC_DIR)/tests/*.c)
TEST_OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(TEST_SRCS)))
TEST_BINS = $(basename $(TEST_OBJS))


.PHONY: all
all: $(LIB) $(TEST_BINS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@ 


# Rule for making the library

.PHONY: libmatrix
libmatrix: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^
	chmod 644 $@


# Tests

.PHONY: tests
tests: $(TEST_BINS)

$(TEST_BINS): $(TEST_OBJS) $(LIB)
	$(CC) $(CFLAGS) $< -o $@ -L $(BUILD_DIR) -lmatrix
	chmod 755 $@
