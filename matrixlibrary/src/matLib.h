#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#ifndef MATLIB_H
#define MATLIB_H

#define FLOAT

#ifdef FLOAT
typedef float value;
#define FORMAT_STRING "%f "
#endif

#ifndef FLOAT
typedef int value;
#define FORMAT_STRING "%i "
#endif

 struct matrix {
	 int columns;
	 int rows;
	 size_t size;
	 value *start;
 };
typedef struct matrix matrix;
matrix* create_matrix(int row,int col);
bool insert_value(value insert,int row,int col,matrix* mat);
bool check_boundaries(int row,int col,matrix* mal);
void insert_value_without_check(value insert, int row, int col, matrix* mat);
value get_value(int row, int col, matrix* mat);
value get_value_without_check(int row, int col, matrix* mat);
bool add_matrices(matrix* a, matrix* b, matrix* c);
bool subtract_matrices(matrix* a, matrix* b,matrix* c);
value sum_of_row(int row, matrix* mat);
value sum_of_column(int column, matrix* mat) ;
bool get_row_vector(int row,matrix* a,matrix* b);
bool get_column_vector(int column, matrix* a,matrix* b) ;
bool multiply_matrices(matrix* a, matrix* b,matrix* c);
bool transpose_matrix(matrix* a, matrix*b);
bool inverse_of_2x2_matrix(matrix* a,matrix* b);
void multiply_matrix_with_scalar(int scal,matrix* mat);
bool get_sub_matrix(int start_row,int end_row,int start_col,int end_col,matrix* a,matrix* b);
bool insert_array(value arr[], matrix* mat);
bool compare_matrices(matrix* a,matrix* b);
bool is_matrix(matrix* a,matrix* b);
void print_matrix(matrix* mat);
int raise(int base, int exp);
matrix* prime_factorization(int number);
bool insert_column_vector(int column,matrix *a,matrix* b);
void free_matrix(matrix* mat);
bool pivot_column(int column, matrix* a,matrix* b);
bool switch_rows(int row1,int row2,matrix* a);
bool insert_row_vector(int row,matrix* a,matrix* b);
void multiply_row_with_scalar(int scal,int row,matrix* mat);
void divide_row_with_scalar(int scal,int row,matrix* mat);
void multiply_column_with_scalar(int scal,int col,matrix* mat);
void add_rows(int row1,int row2,matrix* a);
int lowest_common_denominator(int x,int y);
int greatest_common_denominator(int x,int y);
int gcd_row(int row, matrix* a);

#endif /* MATLIB_H */
