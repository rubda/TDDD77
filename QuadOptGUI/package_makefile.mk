SOLVER_SRCS = $(wildcard quadopt/src/*.c)

CC = gcc
CFLAGS = -O3 --std=c99

.PHONY: solution
solution:
	$(CC) $(CFLAGS) $(SOLVER_SRCS) solution.c -o solution -I quadopt/include -I matrixlibrary/include -L matrixlibrary/obj -lmatrix
