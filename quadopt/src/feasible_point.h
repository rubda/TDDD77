#ifndef FEASIBLE_POINT_H
#define FEASIBLE_POINT_H

#include "problem.h"
#include <matLib.h>

/* Checks if a point is feasible subject to the constraints in a problem */
bool is_feasible_point(matrix* z, problem* prob);

/* Calculates a feasible starting point for a problem */
bool find_starting_point(problem* prob);

#endif /* FEASIBLE_POINT_H */
