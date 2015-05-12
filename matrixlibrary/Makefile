SRC_DIR = src
INCLUDE_DIR = include

CC = gcc
CFLAGS = -Wall -Wextra -pedantic --std=c99

SRCS = $(wildcard $(SRC_DIR)/*.c)
LIB_NAME = matLib
LIB = $(SRC_DIR)/lib$(LIB_NAME).a

TEST_SRCS = $(wildcard $(SRC_DIR)/tests/*.c)
TEST_BINS = $(subst .c,,$(TEST_SRCS))

.PHONY: all
all: $(LIB) $(TEST_BINS)

$(SRC_DIR)/tests/%: $(SRC_DIR)/tests/%.c $(wildcard $(INCLUDE_DIR)/*.h) $(LIB)
	$(CC) $(CFLAGS)  $< -o $@ -I $(INCLUDE_DIR) -L $(SRC_DIR) -lmatLib

$(LIB): $(SRC_DIR)/matLib.c $(wildcard $(INCLUDE_DIR)/*.h)
	$(CC) $(CFLAGS) -c $< -o $(subst .c,.o, $<) -fPIC -I $(INCLUDE_DIR) 
	ar rcs $@ $(subst .c,.o, $<)

.PHONY: clean
clean:
	rm -f $(TEST_BINS)
	rm -f $(LIB)
	rm -f $(SRC_DIR)/$(LIB_NAME).o
