#ifndef MATLIB_H
#define MATLIB_H

/*
  Author: Martin Soderen
  Email: martin.soderen@gmail.com
  Date: 2015-03-05
  Description: This is the headerfile for the matLib-library which is used to handle matrices.
*/

/** Only standardlibraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/** Uncomment which mode you want the library to run in */
/* #define INT */
/* #define FLOAT */
/* #define DOUBLE */
#define DOUBLE

/** Setup for the preprocessor depending on mode */
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
#define FORMAT_STRING "%f "
#define PRECISION 0.0001
#endif

#ifdef QUAD
typedef long double value;
#define FORMAT_STRING "%Lf "
#define PRECISION 0.000001
#endif

/** Uncomment to allow parallel operations */
/* #define PARALLEL */

/** This is the core-struct in this library. All matrix-operations are based on this Struct. */
struct matrix {
  size_t columns;
  size_t rows;
  size_t size;
  value *start;
  bool diagonals;
};

/* Matrix instead of struct matrix */
typedef struct matrix matrix;

#ifdef PARALLEL

#include <pthread.h>
#include <semaphore.h>

/** Sets on how many cores calculations can run on */
static const int number_of_cores=32;

/** Used to protect the counter, remember to run initialize_parallelization */
pthread_mutex_t lock;

int* thread_counter;

/* Used in strassen parallel to pass matrices to target functions */
struct matrices{
  matrix* one;
  matrix* two;
  matrix* three;
  matrix* four;
  matrix* five;
};

/* Matrices instead of struct matrices */
typedef struct matrices matrices;

#endif

/** Create a matrix */
matrix* create_matrix(size_t row, size_t col);

/** Is normally not needed for this implementation but might be needed on others */
matrix* create_zero_matrix(size_t row,size_t col);

/** Creates a identity matrix */
matrix* create_identity_matrix(size_t row,size_t col);

/** Calculate the dot product */
value dot_product(matrix* r, matrix* v);

/** Destroy a matrix */
void free_matrix(matrix* mat);

/** Prints the matrix*/
void print_matrix(matrix* mat);

/** Checks if the position exists in the matrix */
bool check_boundaries(size_t row, size_t col, matrix* mat);

/** Insert a array into the matrix */
bool insert_array(value arr[], matrix* mat);

/** Returns true if matrices a and b look the same */
bool compare_matrices(matrix* a, matrix* b);

/** Return true if the matrix are the same */
bool is_matrix(matrix* a, matrix* b);

/** Insert a value into matrix */
bool insert_value(value insert,size_t row, size_t col, matrix* mat);

/** As insert_value without check */
void insert_value_without_check(value insert, size_t row, size_t col, matrix* mat);

/** Get a value from matrix */
value get_value(size_t row, size_t col, matrix* mat);

/** As get_value without check */
value get_value_without_check(size_t row, size_t col, matrix* mat);

/** Adds a and b into c */
bool add_matrices(matrix* a, matrix* b, matrix* c);

/** Adds a and b by returning a pointer a matrix with a+b */
matrix* add_matrices_with_return(matrix* a, matrix* b);

/** Subtract a and b into c. c=a-b */
bool subtract_matrices(matrix* a, matrix* b, matrix* c);

/** Subtracts a and b by returning a pointer a matrix with a-b */
matrix* subtract_matrices_with_return(matrix* a, matrix* b);

/** Multiply a and b into c. c=a*b */
bool multiply_matrices(matrix* a, matrix* b, matrix* c);

/* Multiply a and b into c using the naive algorithm. c=a*b */
bool multiply_matrices_naive(matrix* a, matrix* b, matrix* c);

/* Multiply a and b into c. Uses row-major optimization. c=a*b */
bool multiply_matrices_optimized(matrix* a, matrix* b, matrix* c);

/* Multiply a and b using the Strassen algorithm and return a pointer to matrix c. c=a*b */
matrix* strassen_matrices_with_return(matrix* a, matrix* b);

/* Multiply a and b into c using the Strassen algorithm. c=a*b */
bool strassen_matrices(matrix* a, matrix* b, matrix* c);

#ifdef PARALLEL

/* Multiply a and b using the Strassen algorithm in parallel, returns a pointer to c. c=a*b */
matrix* strassen_matrices_parallel_with_return(matrix* a, matrix* b);

/* Help function to strassen parallel */
void *calculation_one(void* arg);

/* Help function to strassen parallel */
void *calculation_two(void* arg);

/* Help function to strassen parallel */
void *calculation_three(void* arg);

/* Help function to strassen parallel */
void *calculation_four(void* arg);

/* Help function to strassen parallel */
void *calculation_five(void* arg);

/* Help function to strassen parallel */
void *calculation_six(void* arg);

/* Help function to strassen parallel */
void *calculation_seven(void* arg);

/* Multiply a and b into c using the Strassen algorithm running in parallel. c=a*b */
bool strassen_matrices_parallel(matrix* a, matrix* b, matrix* c);

/** Initializes parallel variables */
void initialize_parallelization();

/** Free parallel variables */
void deinitialize_parallelization();

/** Used to insure the number of processes */
void start_thread();

/** Used to insure the number of processes */
void exit_thread();

#endif

/** Multiply a and b by returning a pointer to a new matrix with a*b*/
matrix* multiply_matrices_with_return(matrix* a, matrix* b);

/** Returns the determinant of matrix a */
value get_determinant(matrix* a);

/** Calculates the inverse of a and puts it into c */
bool get_inverse(matrix* a, matrix* c);

matrix* get_inverse_of_2x2_with_return(matrix* a);

bool get_inverse_of_2x2(matrix* a,matrix* b);

/** Solves Ax=B */
bool solve_linear(matrix* a,matrix* x, matrix *b);

/** Solves ax=b by returning a pointer to x */
matrix* solve_linear_with_return(matrix* a,matrix *b);

/** Crout algorithm to divide matrix a into l and u that holds a=lu */
bool crout(matrix* a, matrix* l, matrix* u);

/** Solves lux=b using backward and forward substitution */
void forward_backward(matrix* l, matrix* u, matrix* x, matrix* b);

/** If no solution can be found with solve_linear, this function finds the closest one */
void least_square(matrix* a, matrix* x, matrix* b);

/** Gauss eliminates the matrix a */
bool gauss_jordan(matrix* a);

/** Solves the system of linear equations using gauss jordan */
bool gauss_jordan_solver(matrix* a,matrix* x,matrix* b);

/** Returns a matrix with only pivots elements from a  */
matrix* get_matrix_with_only_pivots(matrix* a);

/** Returns the lowest of the two values */
value min(value a,value b);

/** Returns on which row the largest element in the column is after start */
size_t largest_element_in_column_index(size_t column,size_t start,matrix* a);

/** Returns on which row the smallest element in the column is after start */
size_t smallest_element_in_column_index(size_t column,size_t start,matrix* a);

/** Returns on which row the first nonezero element is in the column is after start returns -1
   if no nonezero element is found */
size_t first_nonezero_in_column_index(size_t column, size_t start, matrix* a);

/** Returns on which column the first nonezero element is in the column is after start returns -1
   if no nonezero element is found */
size_t first_nonezero_in_row_index(size_t row,size_t start, matrix* a);

/** Adds each element in row1 and row 2 and puts the result on row2 */
void add_rows(size_t row1, size_t row2, matrix* a);

/** Transposes matrix a into b */
bool transpose_matrix(matrix* a, matrix*b);

/** Transposes matrix a by returning a pointer to a:s transpose */
matrix* transpose_matrix_with_return(matrix* a);

/** Return the sum of a row in matrix mat */
value sum_of_row(size_t row, matrix* mat);

/** Return the sum of a column in matrix mat */
value sum_of_column(size_t column, matrix* mat);

/** Return the product of a row in matrix mat */
value product_of_row(size_t row, matrix* mat);

/** Return the product of a column in matrix mat */
value product_of_column(size_t column, matrix* mat);

/** Multiplies matrix mat with scalar */
void multiply_matrix_with_scalar(value scal, matrix* mat);

/** Divides matrix mat with scalar */
void divide_matrix_with_scalar(value scal, matrix* mat);

/** Multiplies a row with a scalar */
void multiply_row_with_scalar(value scal, size_t row, matrix* mat);

/** Divides a row with a scalar */
void divide_row_with_scalar(value scal, size_t row, matrix* mat);

/** Multiplies a column with a scalar */
void multiply_column_with_scalar(value scal, size_t col, matrix* mat);

/** Divides a column with a scalar */
void divide_column_with_scalar(value scal, size_t col, matrix* mat);

/** Takes row vector from matrix a and puts it into b */
bool get_row_vector(size_t row, matrix* a, matrix* b);

/** Returns row vector row from matrix a with a pointer to a matrix */
matrix* get_row_vector_with_return(size_t row,matrix* a);

/** Inserts row vector a into b:s row */
bool insert_row_vector(size_t row, matrix* a, matrix* b);

/** Switches rows in a */
bool switch_rows(size_t row1, size_t row2, matrix* a);

/** Takes column vector from matrix a and puts it into b */
bool get_column_vector(size_t column, matrix* a, matrix* b);

/** Takes column vector from matrix a and return a pointer to the row vector*/
matrix* get_column_vector_with_return(size_t column, matrix* a);

/** Inserts column vector a into matrix b at position column */
bool insert_column_vector(size_t column, matrix *a, matrix* b);

/** Get a sub matrix from a */
bool get_sub_matrix(size_t start_row, size_t end_row, size_t start_col, size_t end_col, matrix* a, matrix* b);

/* inserts the submatrix defined by start_row,end_row,start_col,end_col and put it into matrix b */
bool insert_sub_matrix(size_t start_row, size_t end_row, size_t start_col, size_t end_col, matrix* b, matrix* a);

/** Copy and return new matrix. */
matrix* matrix_copy(matrix* source);

/** Copies all the data from matrix A into matrix B */
void matrix_copy_data(matrix* A, matrix* B);

/** Checks if all elements in a matrix is equal to zero */
bool is_zero_matrix(matrix* v);

/** Checks if all elements in a matrix is positive */
bool is_non_negative_matrix(matrix* v);

/** Checks if all elements along the diagonal in a symmetric matrix is positive */
bool is_non_negative_diagonal_matrix(matrix* A);

/** Takes the diagonal in a and puts it into b */
bool get_diagonal(matrix* a,matrix* b);

/** Returns a pointer to a matrix with the derivative of var if the a matrix second order coefficiants */
matrix* derivate_matrix_with_return(size_t var,matrix* a);

/* Fucks shit up */
void transform_to_reduced_row_echelon_form(matrix* M);

/** Return true if b contains value a */
bool matrix_contains(value a,matrix* b);

/** Compare two element values */
int compare_elements(value a, value b);

/** Creates new matrix with zero values */
matrix* get_zero_matrix(size_t rows, size_t columns);

/** Returns the absolute value of a */
value matlib_fabs(value a);

#endif /* MATLIB_H */
