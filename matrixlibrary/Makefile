BUILD_DIR = obj
INCLUDE_DIR = include
SRC_DIR = src

CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -pedantic --std=c99


SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(SRCS)))
LIB = $(BUILD_DIR)/libmatrix.a

TEST_SRCS = $(wildcard $(SRC_DIR)/tests/*.c)
TEST_OBJS = $(patsubst %.c, %.o, $(subst $(SRC_DIR)/, $(BUILD_DIR)/, $(TEST_SRCS)))
TEST_BINS = $(basename $(TEST_OBJS))

ifndef DONT_RUN_TESTS
.PHONY: all
all: $(LIB) test
else
.PHONY: all
all: $(LIB) $(TEST_BINS)
endif


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@ 

$(BUILD_DIR)/matLib.o: $(SRC_DIR)/matLib.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -I $(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/sparse_lib.o: $(SRC_DIR)/sparse_lib.c $(wildcard $(INCLUDE_DIR)/*.h)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -I $(INCLUDE_DIR) -c $< -o $@


# Rule for making the library

.PHONY: libmatrix
libmatrix: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^
	chmod 644 $@


# Tests

.PHONY: test
test: $(TEST_BINS)
	@echo Running tests...
	@$(foreach test_bin, $(TEST_BINS), echo Running: $(test_bin) && $(test_bin) &&) true

$(TEST_BINS): $(TEST_OBJS) $(LIB)
	$(CC) $@.o -o $@ -L $(BUILD_DIR) -lmatrix -pthread -lm
	chmod 755 $@

