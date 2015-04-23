#ifndef SOLVER_H
#define SOLVER_H

/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to reach the optimum value given by the problem.
*/

#include <problem.h>
#include <matLib.h>

/** Removes the active constraint with the most negative lagrange multiplier */
bool remove_constraint(problem* prob);

/** Solves a quadratic problem using the active set method */
matrix* quadopt_solver(problem* prob);

#endif /* SOLVER_H */
