mex -setup C

mex -I../matrixlibrary/include -c ../quadopt/src/solver.c
mex -I../matrixlibrary/include -c ../quadopt/src/work_set.c

mex -I../matrixlibrary/include -c ../matlab/quadopt.c

mex -L../matrixlibrary/obj -lmatrix quadopt.o work_set.o solver.o 


