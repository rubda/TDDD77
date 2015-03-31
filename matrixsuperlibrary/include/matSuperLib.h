#ifndef MATSUPERLIB_H
#define MATSUPERLIB_H

/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-05
  Description: This is the headerfile for the matLib-library which is used to handle matrices.
*/

/* Only standardlibraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <matLib.h>

/* A struct that resembles matrix but contains matrices */
struct matrix_with_matrices {
  int columns;
  int rows;
  size_t size;
  matrix *start;
  bool diagonals;
};

typedef struct matrix_with_matrices matrix_with_matrices;

/* Create a matrix with matrices */
matrix_with_matrices* create_matrix_with_matrices(int row, int col);

#endif /* MATSUPERLIB_H */

