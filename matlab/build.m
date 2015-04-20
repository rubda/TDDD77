mex -setup C

mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/solver.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/work_set.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/problem.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/feasible_point.c
mex -I../matrixlibrary/include -I../quadopt/include -c ../quadopt/src/subproblem.c

mex -I../matrixlibrary/include -I../quadopt/include -c ../matlab/quadopt.c

mex -L../matrixlibrary/obj -lmatrix quadopt.o work_set.o solver.o problem.o feasible_point.o subproblem.o


