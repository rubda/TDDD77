BUILD_DIR = obj
SRC_DIR = src

LIBMATRIX_DIR = ../matrixlibrary/obj
LIBMATRIX_INCLUDE = ../matrixlibrary/include
LIBMATRIX = $(LIBMATRIX_DIR)/libmatrix.a

CC = gcc
CFLAGS = -Wall -pedantic --std=c99

.PHONY: all
all:

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)


# General c-file to object file rule

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(SRC_DIR)/*.h) $(LIBMATRIX)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -L $(LIBMATRIX_DIR) -l $(LIBMATRIX)

.PHONY: $(LIBMATRIX)
$(LIBMATRIX):
	cd ../matrixlibrary ; make libmatrix
