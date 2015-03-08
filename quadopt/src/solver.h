#include "matLib.h"
#include "work_set.h"



//calculate_lagrange();

value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws);

matrix* quadopt_solver(matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy);
