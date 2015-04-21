#ifndef SOLVER_H
#define SOLVER_H

#include <problem.h>
#include <matLib.h>

/** Removes the active constraint with the most negative lagrange multiplier */
bool remove_constraint(problem* prob);

/** Solves a quadratic problem using the active set method */
matrix* quadopt_solver(problem* prob);

#endif /* SOLVER_H */
