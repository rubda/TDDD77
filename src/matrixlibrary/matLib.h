#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef int value;
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
