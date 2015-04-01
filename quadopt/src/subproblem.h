#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include <matLib.h>
#include <work_set.h>

void solve_supbroblem(matrix* Ain, matrix* G, matrix* gk, matrix* d, matrix* z, matrix* p, matrix* lagrange, work_set* ws);

#endif /* SUBPROBLEM_H */
