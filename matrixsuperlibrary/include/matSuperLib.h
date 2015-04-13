#ifndef MATSUPERLIB_H
#define MATSUPERLIB_H

/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-05
  Description: This is the headerfile for the matSuperLib-library which is used to handle matrices containing matrices.
*/

/* Only standardlibraries and matLib */
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
  matrix** start;
  bool diagonals;
};

typedef struct matrix_with_matrices matrix_m;

/* Creates a matrix row x col. So create_matrix(2,2) will return
 * a pointer to a matrix with 2 rows and 2 columns.
 * Returns NULL if row or col are incorrect*/
matrix_m* create_matrix_m(int row, int col);

/* Frees the matrix_m */
void free_matrix_m(matrix_m* a);

/*prints all the matrices*/
void print_matrix_m(matrix_m* mat);

/* Inserts matrix at pos row,col in matrix mat */
bool insert_matrix(int row, int col,matrix* insert, matrix_m* mat);

/* Returns value on location row,col in matrix mat.
 * WARNING: Only returns 0 if outside of the matrix */
matrix* get_matrix(int row, int col, matrix_m* mat);

/* Adds a and b into c. c=a+b */
bool add_matrices_m(matrix_m* a, matrix_m* b, matrix_m* c);

/* Adds a and b into c. c=a+b */
matrix_m* add_matrices_m_with_return(matrix_m* a, matrix_m* b);

/* Subtracts a and b into c. c=a+b */
bool subtract_matrices_m(matrix_m* a, matrix_m* b, matrix_m* c);

/* Subtracts a and b into c. c=a+b */
matrix_m* subtract_matrices_m_with_return(matrix_m* a, matrix_m* b);

/* Multiply a and b into c. c=a*b */
bool multiply_matrices_m(matrix_m* a, matrix_m* b, matrix_m* c);

/* Multiply a and b by returning a pointer to c. c=a*b */
matrix_m* multiply_matrices_m_with_return(matrix_m* a, matrix_m* b);

/* Transposes matrix a into b */
bool transpose_matrix_m(matrix_m* a, matrix_m*b);

/* Transposes matrix a into b */
matrix_m* transpose_matrix_m_with_return(matrix_m* a);

/* creates a matrix_m with only 1x1 matrices with values from a*/
matrix_m* create_matrix_m_from_matrix(matrix* a);



#endif /* MATSUPERLIB_H */


