#ifndef PROBLEM_H
#define PROBLEM_H

/*
  Author: Ruben Das
  Email: rubda680@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to retrieve values from the matrices given.
*/

#include <matLib.h>
#include <work_set.h>

/** Allocates the problem and sets all necessary variables */
struct problem{
  /* Optimization function */
  matrix* Q; /**< The matrix containing the quadratic optimization problem. */
  matrix* Q_inv; /**< Q inverse. */

  matrix* q; /**< The matrix containing the linear optimization problem. */

  /* Equality constraints */
  int equality_count; /**< Number of equality constraints (Rows in the equality constraints matrices). */
  matrix* E;
  matrix* h;

  /* Larger-than constraints */
  int inequality_count;
  matrix* F;
  matrix* g;

  /* All constraints */
  matrix* A;
  matrix* b;
  int constraints_count;

  /* Variables */
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

  /* Max iterations/µs restrictions */
  int max_iter;
  int max_micro_sec;
  bool check_time;
};

typedef struct problem problem;

/** Puts matrices to a problem struct */
problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, matrix* F, matrix* g, 
			matrix* z0, int max_iter, int max_micro_sec);

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

/** Exits solver if maximal iterations or microseconds have been fullfilled */
bool time_to_exit(problem* prob, double time_spent);

#endif /* PROBLEM_H */
