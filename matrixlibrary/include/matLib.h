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
/*#define INT*/
#define FLOAT
/*#define DOUBLE*/
/*#define QUAD*/

/* Setup for the preprocessor depending on mode */
#ifdef INT
typedef int value;
#define FORMAT_STRING "%i "
#define PRECISION 0
#endif

#ifdef FLOAT
typedef float value;
#define FORMAT_STRING "%f "
#define PRECISION 0.01
#endif

#ifdef DOUBLE
typedef double value;
#define FORMAT_STRING "%lf "
#define PRECISION 0.0001
#endif

#ifdef QUAD
typedef long double value;
#define FORMAT_STRING "%Lf "
#define PRECISION 0.000001
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

/* Is normally not needed for this implementation but might be needed on others */
matrix* create_zero_matrix(int row,int col);

/* Creates a identity matrix */
matrix* create_identity_matrix(int row,int col);

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

/* Adds a and b by returning a pointer a matrix with a+b */
matrix* add_matrices_with_return(matrix* a, matrix* b);

/* Subtract a and b into c. c=a-b */
bool subtract_matrices(matrix* a, matrix* b, matrix* c);

/* Subtracts a and b by returning a pointer a matrix with a-b */
matrix* subtract_matrices_with_return(matrix* a, matrix* b);

/* Multiply a and b into c. c=a*b */
bool multiply_matrices(matrix* a, matrix* b, matrix* c);

/* Multiply a and b into c. Uses row-major optimization. c=a*b */
bool multiply_matrices_optimized(matrix* a, matrix* b, matrix* c);

/* Multiply a and b by returning a pointer to a new matrix with a*b*/
matrix* multiply_matrices_with_return(matrix* a, matrix* b);

/* Returns the determinant of matrix a */
value get_determinant(matrix* a);

/* Calculates the inverse of a and puts it into c */
bool get_inverse(matrix* a, matrix* c);

/* Solves Ax=B */
bool solve_linear(matrix* a,matrix* x, matrix *b);

/* Solves ax=b by returning a pointer to x */
matrix* solve_linear_with_return(matrix* a,matrix *b);

/* Crout algorithm to divide matrix a into l and u that holds a=lu */
bool crout(matrix* a, matrix* l, matrix* u);

/* Solves lux=b using backward and forward substitution */
void forward_backward(matrix* l, matrix* u, matrix* x, matrix* b);

/* If no solution can be found with solve_linear, this function finds the closest one */
void least_square(matrix* a, matrix* x, matrix* b);

/* Gauss eliminates the matrix a */
bool gauss_jordan(matrix* a);

/* Returns a matrix with only pivots elements from a  */
matrix* get_matrix_with_only_pivots(matrix* a);

/* Returns the lowest of the two values */
value min(value a,value b);

/* Returns on which row the largest element in the column is after start */
int largest_element_in_column_index(int column,int start,matrix* a);

/* Returns on which row the smallest element in the column is after start */
int smallest_element_in_column_index(int column,int start,matrix* a);

/* Returns on which row the first nonezero element is in the column is after start returns -1
 * if no nonezero element is found */
int first_nonezero_in_column_index(int column, int start, matrix* a);

/* Returns on which column the first nonezero element is in the column is after start returns -1
 * if no nonezero element is found */
int first_nonezero_in_row_index(int row,int start, matrix* a);

/* Adds each element in row1 and row 2 and puts the result on row2 */
void add_rows(int row1, int row2, matrix* a);

/*transposes matrix a into b */
bool transpose_matrix(matrix* a, matrix*b);

/* Transposes matrix a by returning a pointer to a:s transpose */
matrix* transpose_matrix_with_return(matrix* a);

/* Return the sum of a row in matrix mat */
value sum_of_row(int row, matrix* mat);

/* Return the sum of a column in matrix mat */
value sum_of_column(int column, matrix* mat);

/* Return the product of a row in matrix mat */
value product_of_row(int row, matrix* mat);

/* Return the product of a column in matrix mat */
value product_of_column(int column, matrix* mat);

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

/* Returns row vector row from matrix a with a pointer to a matrix */
matrix* get_row_vector_with_return(int row,matrix* a);

/* Inserts row vector a into b:s row */
bool insert_row_vector(int row, matrix* a, matrix* b);

/* Switches rows in a */
bool switch_rows(int row1, int row2, matrix* a);

/* Takes column vector from matrix a and puts it into b */
bool get_column_vector(int column, matrix* a, matrix* b);

/* Takes column vector from matrix a and return a pointer to the row vector*/
matrix* get_column_vector_with_return(int column, matrix* a);

/* Inserts column vector a into matrix b at position column */
bool insert_column_vector(int column, matrix *a, matrix* b);

/* Get a sub matrix from a */
bool get_sub_matrix(int start_row, int end_row, int start_col, int end_col, matrix* a, matrix* b);

/* Copy and return new matrix. */
matrix* matrix_copy(matrix* source);

/* Copies all the data from matrix A into matrix B */
void matrix_copy_data(matrix* A, matrix* B);

/* Checks if all elements in a matrix is equal to zero */
bool is_zero_matrix(matrix* v);

/* Checks if all elements in a matrix is positive */
bool is_non_negative_matrix(matrix* v);

/* Checks if all elements along the diagonal in a symmetric matrix is positive */
bool is_non_negative_diagonal_matrix(matrix* A);

/* Takes the diagonal in a and puts it into b */
bool get_diagonal(matrix* a,matrix* b);

/* Returns a pointer to a matrix with the derivative of var if the a matrix second order coefficiants */
matrix* derivate_matrix_with_return(int var,matrix* a);

/* Fucks shit up */
void transform_to_reduced_row_echelon_form(matrix* M);

/* return true if b contains value a */
bool matrix_contains(value a,matrix* b);

/* compare two element values */
bool compare_elements(value a, value b);

/* Creates new matrix with zero values */
matrix* get_zero_matrix(int rows, int columns);

#endif /* MATLIB_H */
