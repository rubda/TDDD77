#ifndef FEASIBLE_POINT_H
#define FEASIBLE_POINT_H

/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to determine if a point is feasible.
               It also contains functions that can calculate a feasible point with given constraints.
*/

#include <problem.h>
#include <matLib.h>

/** Checks if a point is feasible subject to the constraints in a problem */
bool is_feasible_point(matrix* z, problem* prob);

/** Calculates a feasible starting point for a problem */
bool find_starting_point(problem* prob);

#endif /* FEASIBLE_POINT_H */
