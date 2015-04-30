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
bool check_dim_A(matrix* A, size_t card_x);
bool check_dim_B(matrix* B, size_t card_x, size_t card_u);
bool insert_identity_matrices(matrix* E, size_t card_x);
bool insert_A_matrices(matrix* E, matrix* A);
bool insert_B_matrices(matrix* E, matrix* B, size_t N);

/** Dynamic constraints (A and B with initial values K) transforms to equality constraints (E and h).*/
bool dyn_2_eq(matrix* A, matrix *B, matrix* k, matrix* E, matrix* h, size_t card_x, size_t card_u){
  if(!check_dim_k(k, card_x)) return false;
  if(!check_dim_h(h)) return false;
  size_t N = (h->rows - 2) / 2;
  if(!check_dim_E(E, card_x, card_u, N)) return false;
  if(!check_dim_A(A, card_x)) return false;
  if(!check_dim_B(B, card_x, card_u)) return false;

  if(!insert_sub_matrix(1, k->rows, 1, 1, k, h)) return false;
  if(!insert_identity_matrices(E, card_x)) return false;
  multiply_matrix_with_scalar(-1, A);
  if(!insert_A_matrices(E, A)) return false;
  if(!insert_B_matrices(E, B, N)) return false;

  return true;
}

bool insert_identity_matrices(matrix* E, size_t card_x){
  matrix* id_matrix = create_identity_matrix(card_x, card_x);
  size_t start_col = 1;
  for(int start_row = 1; start_row < E->rows; start_row += card_x){
    size_t end_row = start_row + card_x - 1;
    size_t end_col = start_col + card_x - 1;
    if(!insert_sub_matrix(start_row, end_row, start_col, end_col, id_matrix, E)){
      free_matrix(id_matrix);
      return false;
    }
    start_col += card_x;
  }

  free_matrix(id_matrix);
  return true;
}

bool insert_A_matrices(matrix* E, matrix* A){
  size_t start_col = 1;
  for(int start_row = 1 + A->rows; start_row < E->rows; start_row += A->rows){
    size_t end_row = start_row + A->rows - 1;
    size_t end_col = start_col + A->columns - 1;
    if(!insert_sub_matrix(start_row, end_row, start_col, end_col, A, E)){
      return false;
    }
    start_col += A->columns;
  }

  return true;
}

bool insert_B_matrices(matrix* E, matrix* B, size_t N){
  size_t start_col = N*B->rows + B->rows + 1;
  for(int start_row = 1 + B->rows; start_row < E->rows; start_row += B->rows){
    size_t end_row = start_row + B->rows - 1;
    size_t end_col = start_col + B->columns - 1;
    if(!insert_sub_matrix(start_row, end_row, start_col, end_col, B, E)){
      return false;
    }
    start_col += B->columns;
  }

  return true;
}

bool check_dim_k(matrix* k, size_t card_x){
  if(k->rows != (int)card_x) return false;
  return true;
}

bool check_dim_h(matrix* h){
  if(h->columns != 1) return false;
  if(h->rows % 2 != 0) return false;
  return true;
}

bool check_dim_E(matrix* E, size_t card_x, size_t card_u, size_t N){
  if(E->rows != (int)(2*N+2)) return false;
  if(E->columns != (int)((card_x + card_u)*N + 2)) return false;
  return true;
}

bool check_dim_A(matrix* A, size_t card_x){
  if(A->rows != (int)card_x) return false;
  if(A->columns != (int)card_x) return false;
  return true;
}

bool check_dim_B(matrix* B, size_t card_x, size_t card_u){
  if(B->rows != (int)card_x) return false;
  if(B->columns != (int)card_u) return false;
  return true;
}

