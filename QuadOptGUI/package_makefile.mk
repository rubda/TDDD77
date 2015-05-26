SOLVER_SRCS = $(wildcard quadopt/src/*.c)

CC = gcc
CFLAGS = -O3 --std=c99

solution:
	$(CC) $(CFLAGS) $(SOLVER_SRCS) result.c -o solution -I quadopt/include -I matrixlibrary/include -L matrixlibrary/obj -lmatrix
