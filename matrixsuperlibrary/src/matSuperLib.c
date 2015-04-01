/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-31
  Description: This is the matSuperLib-library which is used to handle matrices containing matrices.
*/

/* This is the only include that is allowed in this file */
#include <matSuperLib.h>

/* a 3 x 3 matrix created with create_matrix_with_matrices(3,3);
 * 		column 	1	2	3
 * row
 * 1			[a]	[b]	[c]
 * 2			[d]	[e]	[f]
 * 3			[g]	[h]	[i]
 *
 * The rows and columns are 1-indexed
 * */

/* Creates a matrix row x col. So create_matrix(2,2) will return
 * a pointer to a matrix with 2 rows and 2 columns.
 * Returns NULL if row or col are incorrect*/
matrix_with_matrices* create_matrix_with_matrices(int row, int col) {
  if (row < 1 || col < 1) {
    return NULL;
  }

  matrix_with_matrices* mal = (matrix_with_matrices *) malloc(sizeof(matrix_with_matrices));
  mal->columns = col;
  mal->rows = row;
  mal->size = row * col;
  mal->start =  (value **) malloc(col * row * sizeof(matrix*));
  return mal;
}

/*prints all the matrices*/
void print_matrix_with_matrices(matrix_with_matrices* mat) {
  printf("\n");
  int current_row = 1;
  int current_column = 1;
  matrix* temp = get_matrix(1, 1, mat);
  int matrix_row = temp->rows;
  int matrix_column = temp->columns;
  int rows = mat->rows * temp->rows;
  int columns = mat->columns * temp->columns;
  printf("|");
  for (int i = 1; i <= rows; i++) {
    for (int j = 1; j <= columns; j++) {
      if (current_column > matrix_column) {
        current_column = 1;
        printf("|");
      }
      temp = get_matrix(i / (matrix_row + 1) + 1, j / (matrix_column + 1) + 1,mat);
      printf(FORMAT_STRING,get_value_without_check(current_row, current_column, temp));
      current_column++;

    }
    current_row++;
    if (current_row > matrix_row) {
      current_row = 1;
      printf("|");
      printf("\n");
      for (int k=0;k<rows;k++){
      printf("----------");
      }
      printf("\n");
    }
    else{
      printf("|");
      printf("\n");
    }
  }
  printf("\n");
}

/* Inserts matrix at pos row,col in matrix mat */
bool insert_matrix(int row, int col,matrix* insert, matrix_with_matrices* mat) {
  if (row<0||col<0||row>mat->columns||col>mat->rows){
    return false;
  }
  *(mat->start+mat->columns * (row - 1)  + (col - 1))=insert;
  return true;
}

/* Returns value on location row,col in matrix mat.
 * WARNING: Only returns 0 if outside of the matrix */
matrix* get_matrix(int row, int col, matrix_with_matrices* mat) {
  return *(mat->start+mat->columns * (row - 1)  + (col - 1));
}


