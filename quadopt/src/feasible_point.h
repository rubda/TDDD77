#ifndef FEASIBLE_POINT_H
#define FEASIBLE_POINT_H

#include "problem.h"
#include <matLib.h>

bool is_feasible_point(matrix* z, problem* prob);

bool find_starting_point(problem* prob);

#endif /* FEASIBLE_POINT_H */
