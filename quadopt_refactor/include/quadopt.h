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
  int *active_conditions;
  bool point_set;
  value *current_point;
};

/* matrix instead of struct matrix */
typedef struct problem problem;

/* Create a problem */
problem* create_problem(matrix* G,matrix* g,matrix* A,matrix* b);

/* Present the problem struct */
void present_problem(problem* prob);

/* Finds the initial start point */
void find_start_point(problem* prob);

#endif /* QUADOPT_H */
