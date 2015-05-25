mex -setup C

mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/solver.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/work_set.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/problem.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/subproblem.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/simplex.c

mex -I../matrixlibrary/include -I../quadopt/include -c ../matlab/quadopt.c

mex -L../matrixlibrary/obj -lmatrix quadopt.o work_set.o solver.o problem.o subproblem.o simplex.o


