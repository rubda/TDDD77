/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-05
  Description: This is the matLib-library which is used to handle matrices.
*/

/* This is the only include that is allowed in this file */
#include <matSuperLib.h>

/* a 3 x 3 matrix created with create_matrix(3,3);
 * 		column 	1	2	3
 * row
 * 1			a	b	c
 * 2			d	e	f
 * 3			g	h	i
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
  mal->start = (matrix *) malloc(col * row * sizeof(matrix*));
  return mal;
}

/* Inserts value at pos rowm,col in matrix mat */
bool insert_matrix(matrix* insert,int row, int col, matrix_with_matrices* mat) {
  if (!check_boundaries(row, col, mat)) {
    return false;
  }
  matrix* start = mat->start + mat->columns * (row - 1)  + (col - 1);
  start = insert;
  return true;
}

/* Returns value on location row,col in matrix mat.
 * WARNING: Only returns 0 if outside of the matrix */
matrix* get_matrix(int row, int col, matrix_with_matrices* mat) {
  if (!check_boundaries(row, col, mat)) {
    return false;
  }
  return (mat->start + mat->columns * (row - 1) + (col - 1));
}

