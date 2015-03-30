#ifndef MATLIB_H
#define MATLIB_H

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

/* Uncomment which mode you want the library to run in */
//#define INT
#define FLOAT
//#define DOUBLE
//#define QUAD

/* Setup for the preprocessor depending on mode */
#ifdef INT
typedef int value;
#define FORMAT_STRING "%i "
#endif

#ifdef FLOAT
typedef float value;
#define FORMAT_STRING "%f "
#endif

#ifdef DOUBLE
typedef double value;
#define FORMAT_STRING "%lf "
#endif

#ifdef QUAD
typedef long double value;
#define FORMAT_STRING "%Lf "
#endif

/* This is the core-struct in this library. All matrix-operations are based on this Struct. */
struct matrix {
	int columns;
	int rows;
	size_t size;
	value *start;
	bool diagonals;
};

/* matrix instead of struct matrix */
typedef struct matrix matrix;

/* Create a matrix */
matrix* create_matrix(int row, int col);

/* calculate the dot product */
value dot_product(matrix* r, matrix* v);

/* Destroy a matrix */
void free_matrix(matrix* mat);

/*prints the matrix*/
void print_matrix(matrix* mat);

/* Checks if the position exists in the matrix */
bool check_boundaries(int row, int col, matrix* mat);

/* Insert a array into the matrix */
bool insert_array(value arr[], matrix* mat);

/*returns true if matrices a and b look the same*/
bool compare_matrices(matrix* a, matrix* b);

/* Return true if the matrix are the same */
bool is_matrix(matrix* a, matrix* b);

/* Insert a value into matrix */
bool insert_value(value insert,int row, int col, matrix* mat);

/* As insert_value without check */
void insert_value_without_check(value insert, int row, int col, matrix* mat);

/* Get a value from matrix */
value get_value(int row, int col, matrix* mat);

/* As get_value without check */
value get_value_without_check(int row, int col, matrix* mat);

/* Adds a and b into c */
bool add_matrices(matrix* a, matrix* b, matrix* c);

/* Subtract a and b into c. c=a-b */
bool subtract_matrices(matrix* a, matrix* b, matrix* c);

/* Multiply a and b into c. c=a*b */
bool multiply_matrices(matrix* a, matrix* b, matrix* c);

/* Solves Ax=B */
bool solve_linear(matrix* a,matrix* x, matrix *b);

/* Crout algorithm to divide matrix a into l and u that holds a=lu */
bool crout(matrix* a, matrix* l, matrix* u);

/* Solves lux=b using backward and forward substitution */
void forward_backward(matrix* l, matrix* u, matrix* x, matrix* b);

/* If no solution can be found with solve_linear, this function finds the closest one */
void least_square(matrix* a, matrix* x, matrix* b);

/* Adds each element in row1 and row 2 and puts the result on row2 */
void add_rows(int row1, int row2, matrix* a);

/*transposes matrix a into b */
bool transpose_matrix(matrix* a, matrix*b);

/* Return the sum of a row in matrix mat */
value sum_of_row(int row, matrix* mat);

/* Return the sum of a column in matrix mat */
value sum_of_column(int column, matrix* mat);

/* Multiplies matrix mat with scalar */
void multiply_matrix_with_scalar(value scal, matrix* mat);

/* Divides matrix mat with scalar */
void divide_matrix_with_scalar(value scal, matrix* mat);

/* Multiplies a row with a scalar */
void multiply_row_with_scalar(value scal, int row, matrix* mat);

/* Divides a row with a scalar */
void divide_row_with_scalar(value scal, int row, matrix* mat);

/* Multiplies a column with a scalar */
void multiply_column_with_scalar(value scal, int col, matrix* mat);

/* Divides a column with a scalar */
void divide_column_with_scalar(value scal, int col, matrix* mat);

/* Takes row vector from matrix a and puts it into b */
bool get_row_vector(int row, matrix* a, matrix* b);

/* Inserts row vector a into b:s row */
bool insert_row_vector(int row, matrix* a, matrix* b);

/* Switches rows in a */
bool switch_rows(int row1, int row2, matrix* a);

/* Takes column vector from matrix a and puts it into b */
bool get_column_vector(int column, matrix* a, matrix* b);

/* Inserts column vector a into matrix b at position column */
bool insert_column_vector(int column, matrix *a, matrix* b);

/* Get a sub matrix from a */
bool get_sub_matrix(int start_row, int end_row, int start_col, int end_col, matrix* a, matrix* b);

/* Copy and return new matrix. */
matrix* matrix_copy(matrix* source);

/* Cpoies all the data from matrix A into matrix B */
void matrix_copy_data(matrix* A, matrix* B);

/* checks if all elements in a matrix is equal to zero */
bool is_zero_matrix(matrix* v);

/* checks if all elements in a matrix is positive */
bool is_non_negative_matrix(matrix* v);

/* checks if all elements along the diagonal in a symmetric matrix is positiv */
bool is_non_negative_diagonal_matrix(matrix* A);


void transform_to_reduced_row_echelon_form(matrix* M);


#endif /* MATLIB_H */
