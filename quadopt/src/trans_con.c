/*
  Author: Alexander Yngve
  Email: aleyn573@student.liu.se
  Date: 2015-04-29
  Description: This file containts the necessary functions to transform dynamic
               constraints to normal optimization constraints.
*/

#include <trans_con.h>

/** Dynamic constraints (A and B with initial values K) transforms to equality constraints (E and h).*/
bool dyn_2_eq(matrix* A, matrix *B, matrix* k, matrix* E, matrix* h){
  if(!insert_sub_matrix(1, k->rows, 1, 1, k, h)) return false;

  return true;
}
