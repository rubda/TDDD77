mex -setup C

mex -I../matrixlibrary/include -c ../quadopt/src/solver.c
mex -I../matrixlibrary/include -c ../quadopt/src/work_set.c
mex -I../matrixlibrary/include -c ../quadopt/src/find_lagrange.c

mex -I../matrixlibrary/include -c quadopt.c

mex -L../matrixlibrary/obj -lmatrix quadopt.o work_set.o find_lagrange.o solver.o 
