#ifndef PROBLEM_H
#define PROBLEM_H

#include <matLib.h>
#include <work_set.h>

/** Allocates the problem and sets all necessary variables */
struct problem{
  /** Optimization function */
  matrix* Q;
  matrix* Q_inv;

  matrix* q;

  /** Equality constraints */
  int equality_count;
  matrix* E;
  matrix* h;

  /** Larger-than constraints */
  int inequality_count;
  matrix* F;
  matrix* g;

  /** All constraints */
  matrix* A;
  matrix* b;
  int constraints_count;

  /** Variables */
  bool has_start_point;
  matrix* z0;

  matrix* z;

  matrix* solution;
  value solution_value;
  bool has_solution;

  matrix* p;
  matrix* gk;
  value step;
  matrix* lagrange;

  work_set* active_set;

  value accuracy;
  unsigned int max_iter;
};

typedef struct problem problem;

/** Puts matrices to a problem struct */
problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, 
			matrix* F, matrix* g, matrix* z0, unsigned int max_iter);

/** Prints the matrices defined in the problem struct */
void print_problem(problem* prob);

/** Deallocates all the problems resources */
void free_problem(problem* prob);

/** Returns a matrix with the currently active constraints */
matrix* get_active_conditions(problem* prob);

/** Returns a matrix with the right hand side of the currently active constraints */
matrix* get_active_conditions_rhs(problem* prob);

/** Calculates the optimum value given by the solution point */
bool get_solution_value(problem* prob);

#endif /* PROBLEM_H */
