#ifndef SOLVER_H
#define SOLVER_H

#include "problem.h"
#include <matLib.h>

bool fill_active_set(problem* prob);

bool remove_constraint(problem* prob);

bool take_step(problem* prob);

matrix* quadopt_solver(problem* prob);

#endif /* SOLVER_H */
