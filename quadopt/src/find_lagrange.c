/*
  Author: Ruben Das
  Email: rubendas.rd@@gmail.com
  Date: 2015-03-10
  Description: This is the find_lagrange-function which is used in the solver.
*/

#include <matLib.h>
#include <work_set.h>
#include <stdbool.h>
#include "find_lagrange.h"

/* Finds the lagrange multipliers and removes constraints */
bool find_lagrange(matrix* g, matrix* A, matrix* d, matrix* z, work_set* w, matrix* lagrange){

  /* Puts all the active conditions to w_mat depending on the work_set */
  matrix* tmp_row = create_matrix(1, A->columns);
  matrix* w_tmp = create_matrix(w->count, A->columns);
  matrix* w_mat = create_matrix(A->columns, w->count); 

  for (int i = 0; i < w->count; i++){
    get_row_vector(w->data[i], A, tmp_row);
    insert_row_vector(i+1, tmp_row, w_tmp);
  }
  transpose_matrix(w_tmp, w_mat);
 
  /* Solves the system W_mat * x = g */
  matrix* solved = create_matrix(w->count, 1);
  solve_linear(w_mat, solved, g);

  /* Finds the smallest lagrange value */
  int min_row = 1;
  value min_value = get_value(1, 1, solved);

  for (int i = 2; i <= w->count; i++){
    value temp = get_value(i, 1, solved);
    if (temp < min_value){
      min_value = temp;
      min_row = i;
    }
  }

  /* Modify lagrange vector */
  for (int i = 0; i < w->count; i++){
    insert_value(get_value(i+1, 1, solved), w->data[i], 1, lagrange);
  }

  /* Removes the smallest lagrange value from the work set */
  work_set_remove(w, w->data[min_row-1]);

  /* Success */
  return true;
}
