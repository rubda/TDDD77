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
#include <sparse_lib.h>

/** Allocates the problem and sets all necessary variables */
struct problem{
  /* Optimization function */
  matrix* Q; /**< The matrix containing the quadratic optimization problem. */
  matrix* Q_inv; /**< Q inverse. */

  sparse_matrix* sparse_Q;
  sparse_matrix* sparse_Q_inv;

  bool is_sparse;

  matrix* q; /**< The matrix containing the linear optimization problem. */

  size_t variable_count; /**< The number of variables in the problem. */

  /* Equality constraints */
  size_t equality_count; /**< Number of equality constraints (Rows in the equality constraints matrices). */
  matrix* E; /**< Equality constraints left-hand side coefficient. */
  sparse_matrix* sparse_E;

  matrix* h; /**< Equality constraints right-hand side constraint. */

  /* Larger-than constraints */
  size_t inequality_count; /**< Number of larger-than constraints (Rows in the larger-than constraints matrices). */
  matrix* F; /**< Larger-than constraints left-hand side coefficient. */
  sparse_matrix* sparse_F;

  matrix* g; /**< Larger-than constraints right-hand side constraint. */

  /* All constraints */
  matrix* A; /**< All constraints left-hand side coefficients. */  
  sparse_matrix** sparse_A;

  matrix* b; /**< All constraints right-hand side constraints. */
  size_t constraints_count; /**< Total number of constraints. */

  /* Variables */
  bool has_start_point;
  matrix* z0; /**< The starting point for the solution. */

  matrix* z; /**< The current point in the solution. */

  matrix* solution; /**< The final point in the solution. */
  value solution_value; /**< The value of the solution point. */
  bool has_solution;

  matrix* p; /**< Current step direction towards the solution. */
  matrix* gk; /**< gk = Qz + q, help matrix for the subproblem.
  * @see Q
  * @see z
  * @see q
  */
  value step; /**< How far we will step towards the solution. */
  matrix* lagrange; /**< The lagrange multipliers. */

  work_set* active_set; /**< The active constraints. */

  value accuracy;

  /* Max iterations/micro_sec restrictions */
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

/** Returns a sparse matrix with the currently active constraints */
sparse_matrix* get_sparse_active_conditions(problem* prob);

/** Returns a matrix with the right hand side of the currently active constraints */
matrix* get_active_conditions_rhs(problem* prob);

/** Calculates the optimum value given by the solution point */
bool get_solution_value(problem* prob);

/** Prints optimal point and optimal value */
void print_solution(problem* prob);

/** Exits solver if maximal iterations or microseconds have been fullfilled */
bool time_to_exit(problem* prob, double time_spent);

/** Checks if a point is feasible subject to the constraints in a problem */
bool is_feasible_point(matrix* z, problem* prob);

#endif /* PROBLEM_H */
