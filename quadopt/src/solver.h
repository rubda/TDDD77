#ifndef SOLVER_H
#define SOLVER_H

#include "problem.h"
#include <matLib.h>

/* Fills the active set according to the current position */
bool fill_active_set(problem* prob);

/* Removes the active constraint with the most negative lagrange multiplier */
bool remove_constraint(problem* prob);

/* Calculates and takes the step for active set method */
bool take_step(problem* prob);

/* Solves a quadratic problem using the active set method */
matrix* quadopt_solver(problem* prob);

#endif /* SOLVER_H */
