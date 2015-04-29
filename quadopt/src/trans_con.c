/*
  Author: Alexander Yngve
  Email: aleyn573@student.liu.se
  Date: 2015-04-29
  Description: This file containts the necessary functions to transform dynamic
               constraints to normal optimization constraints.
*/

#include <trans_con.h>

bool check_dim_h(matrix* h);
bool check_dim_k(matrix* k, size_t card_x);
bool check_dim_E(matrix* E, size_t card_x, size_t card_u, size_t N);
bool check_dim_A(matrix* A, size_t card_x, size_t card_u, size_t N);
bool check_dim_B(matrix* B, size_t card_x, size_t card_u, size_t N);

/** Dynamic constraints (A and B with initial values K) transforms to equality constraints (E and h).*/
bool dyn_2_eq(matrix* A, matrix *B, matrix* k, matrix* E, matrix* h, size_t card_x, size_t card_u){
  if(!check_dim_k(k, card_x)) return false;
  if(!check_dim_h(h)) return false;
  size_t N = (h->rows - 2) / 2;
  if(!check_dim_E(E, card_x, card_u, N)) return false;
  if(!check_dim_A(A, card_x, card_u, N)) return false;
  if(!check_dim_B(B, card_x, card_u, N)) return false;


  if(!insert_sub_matrix(1, k->rows, 1, 1, k, h)) return false;

  /* TODO: Fill E matrix */

  return true;
}

bool check_dim_k(matrix* k, size_t card_x){
  if(k->rows != card_x) return false;
  return true;
}

bool check_dim_h(matrix* h){
  if(h->columns != 1) return false;
  if(h->rows % 2 != 0) return false;
  return true;
}

bool check_dim_E(matrix* E, size_t card_x, size_t card_u, size_t N){
  if(E->rows != 2*N+2) return false;
  if(E->columns != (card_x + card_u)*N + 2) return false;
  return true;
}

bool check_dim_A(matrix* A, size_t card_x, size_t card_u, size_t N){
  /* TODO: Checks */
  return true;
}

bool check_dim_B(matrix* B, size_t card_x, size_t card_u, size_t N){
  /* TODO: Checks */
  return true;
}

