#ifndef QUADOPT_H
#define QUADOPT_H
/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-30
  Description: This is the headerfile for the quadOpt-library which is used to solve convex QP problems.
*/

/* Only standardlibraries except for matLib.h */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <matLib.h>
#include <matSuperLib.h>
#include <assert.h>


/* matrix instead of struct matrix */
typedef struct problem problem;
/* This is the core-struct in this library. All QP-operations are based on this Struct. */
struct problem {
  matrix* G;
  matrix* d;
  matrix* A;
  matrix* b;
  matrix* x;
  int number_of_conditions;
  int number_of_active_conditions;
  int number_of_variables;
  matrix* active_conditions;
  /* Used to handle float errors */
  value marginal;
  bool point_set;
  value current_value;
  problem* subproblem;
  bool subproblem_set;
  matrix* variable_dependencies;
  bool variable_dependencies_set;
  int iteration;
  value step;
  matrix* lagrange;
  bool lagrange_set;
  int max_iterations;
  /* used to detect division by zero */
  value max_solution;
};


/* Uncomment to set to debugmode */
#define DEBUG

/* Create a problem */
problem* create_problem(matrix* G,matrix* g,matrix* A,matrix* b);

void step(problem* prob);

void calculate_step(problem* prob);

/* Present the problem struct */
void present_problem(problem* prob);

/* Finds the initial start point */
void find_start_point(problem* prob);

/* Solves the problem struct using active set method */
bool solve_problem(problem* prob);

bool check_conditions_to_quit(problem* prob);

/* Creates a subproblem */
void create_subproblem(problem* prob);

void find_lagrange(problem* prob);

/*return true if the solutionsvector is not a zerovector*/
bool check_subproblem_solution(problem* prob);

/* Solves the sub problem */
void solve_subproblem(problem* prob);

void handle_to_many_conditions(problem* sub);

void handle_to_few_conditions(problem* sub);

/* Calculates the functions current value */
bool calculate_current_value(problem* prob);

/* Sets the active_conditions in the problem struct */
void get_active_conditions(problem* prob);

#endif /* QUADOPT_H */
