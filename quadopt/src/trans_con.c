/*
  Author: Alexander Yngve
  Email: aleyn573@student.liu.se
  Date: 2015-04-29
  Description: This file containts the necessary functions to transform dynamic
               constraints to normal optimization constraints.
*/

#include <trans_con.h>
#include <assert.h>

bool insert_x_identity_matrices(matrix* F, size_t card_x, size_t N);
bool insert_fx(matrix* F, matrix* Fx, size_t card_x, size_t N);
bool insert_u_identity_matrices(matrix* F, size_t card_u, size_t N);
bool fix_g(matrix* g, matrix* gx, matrix* x_lim, matrix* u_lim, size_t N);
bool insert_identity_matrices(matrix* E, size_t card_x);
bool insert_A_matrices(matrix* E, matrix* A);
bool insert_B_matrices(matrix* E, matrix* B, size_t N);

/* Dynamic constraints (A and B with initial values K) transforms to equality constraints (E and h).*/
bool trans_dyn_cons(matrix* A, matrix *B, matrix* k, matrix* E, matrix* h, size_t card_x){
  size_t N = (h->rows - 2) / 2;

  if(!insert_sub_matrix(1, k->rows, 1, 1, k, h)) return false;
  if(!insert_identity_matrices(E, card_x)) return false;
  multiply_matrix_with_scalar(-1, A);
  if(!insert_A_matrices(E, A)) return false;
  if(!insert_B_matrices(E, B, N)) return false;

  return true;
}

bool trans_ineq_cons(matrix* Fx, matrix* gx, matrix* F, matrix* g, size_t card_x, size_t card_u, size_t N, matrix* x_lim, matrix* u_lim){
  insert_x_identity_matrices(F, card_x, N);
  insert_fx(F, Fx, card_x, N);
  insert_u_identity_matrices(F, card_u, N);
  fix_g(g, gx, x_lim, u_lim, N);

  return true;
}

bool insert_x_identity_matrices(matrix* F, size_t card_x, size_t N){
  matrix* id_matrix = create_identity_matrix(card_x, card_x);
  matrix* neg_id_matrix = create_identity_matrix(card_x, card_x);
  multiply_matrix_with_scalar(-1, neg_id_matrix);
  matrix* stuff = create_matrix(2 * card_x, card_x);
  insert_sub_matrix(1, id_matrix->rows, 1, id_matrix->columns, id_matrix, stuff);
  insert_sub_matrix(neg_id_matrix->rows + 1, stuff->rows, 1, neg_id_matrix->columns, neg_id_matrix, stuff);
  free_matrix(id_matrix);
  free_matrix(neg_id_matrix);

  size_t start_row = 1;
  size_t start_col = 1;
  size_t end_row;
  size_t end_col;
  for(start_row = 1; start_row < stuff->rows * N ; start_row += stuff->rows){
    end_row = start_row + stuff->rows - 1;
    end_col = start_col + stuff->columns - 1;
    if(!insert_sub_matrix(start_row, end_row, start_col, end_col, stuff, F)){
      free_matrix(stuff);
      return false;
    }

    start_col += stuff->columns;
  }

  free_matrix(stuff);

  return true;
}

bool insert_fx(matrix* F, matrix* Fx, size_t card_x, size_t N){
  size_t start_row_fx = 2*card_x*N + 1;
  size_t end_row_fx = start_row_fx + Fx->rows - 1;
  size_t start_col_fx = card_x*N+1;
  size_t end_col_fx = start_col_fx + Fx->columns - 1;
  if(!insert_sub_matrix(start_row_fx, end_row_fx, start_col_fx, end_col_fx, Fx, F)){
    return false;
  }

  return true;
}

bool insert_u_identity_matrices(matrix* F, size_t card_u, size_t N){
  matrix* id_matrix = create_identity_matrix(card_u, card_u);
  matrix* neg_id_matrix = create_identity_matrix(card_u, card_u);
  multiply_matrix_with_scalar(-1, neg_id_matrix);
  matrix* stuff = create_matrix(2*card_u, card_u);
  insert_sub_matrix(1, id_matrix->rows, 1, id_matrix->columns, id_matrix, stuff);
  insert_sub_matrix(neg_id_matrix->rows + 1, stuff->rows, 1, neg_id_matrix->columns, neg_id_matrix, stuff);

  free_matrix(id_matrix);
  free_matrix(neg_id_matrix);


  size_t start_col = F->columns - card_u*N + 1;
  for(size_t start_row = F->rows - 2 * card_u * N + 1; start_row < F->rows; start_row += stuff->rows){
    size_t end_row = start_row + stuff->rows - 1;
    size_t end_col = start_col + stuff->columns - 1;
    if(!insert_sub_matrix(start_row, end_row, start_col, end_col, stuff, F)){
      free_matrix(stuff);
      return false;
    }

    start_col += stuff->columns;
  }

  free_matrix(stuff);

  return true;
}

bool fix_g(matrix* g, matrix* gx, matrix* x_lim, matrix* u_lim, size_t N){
  for(size_t start_row = 1; start_row < x_lim->rows*N; start_row += x_lim->rows){
    size_t end_row = start_row + x_lim->rows - 1;
    if(!insert_sub_matrix(start_row, end_row, 1, 1, x_lim, g)){
      return false;
    }
  }

  size_t start_row_gx = x_lim->rows*N + 1;
  size_t end_row_gx = start_row_gx + gx->rows - 1;
  if(!insert_sub_matrix(start_row_gx, end_row_gx, 1, 1, gx, g)){
    return false;
  }

  for(size_t start_row = end_row_gx + 1; start_row < g->rows; start_row += u_lim->rows){
    size_t end_row = start_row + u_lim->rows - 1;
    if(!insert_sub_matrix(start_row, end_row, 1, 1, u_lim, g)){
      return false;
    }
  }

  return true;
}

bool insert_identity_matrices(matrix* E, size_t card_x){
  matrix* id_matrix = create_identity_matrix(card_x, card_x);
  size_t start_col = 1;
  for(size_t start_row = 1; start_row < E->rows; start_row += card_x){
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
  for(size_t start_row = 1 + A->rows; start_row < E->rows; start_row += A->rows){
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
  multiply_matrix_with_scalar(-1, B);
  size_t start_col = N*B->rows + B->rows + 1;
  for(size_t start_row = 1 + B->rows; start_row < E->rows; start_row += B->rows){
    size_t end_row = start_row + B->rows - 1;
    size_t end_col = start_col + B->columns - 1;
    if(!insert_sub_matrix(start_row, end_row, start_col, end_col, B, E)){
      return false;
    }
    start_col += B->columns;
  }

  return true;
}

bool create_objective(int n, matrix* Qin, matrix* P, matrix* R, matrix* Q){

  size_t i, size = Qin->rows*(n+1) + R->rows*n;

  /* insert Qin */
  for(i = 1; i <= n*Qin->rows; i += Qin->rows){
    insert_sub_matrix(i, i+Qin->rows-1, i, i+Qin->columns-1, Qin, Q);
  }

  /* insert P */
  insert_sub_matrix(i, i+P->rows-1, i, i+P->columns-1, P, Q);
  i += P->rows;

  /* insert R */
  for(; i <= size; i+=R->rows){
    insert_sub_matrix(i, i+R->rows-1, i, i+R->columns-1, R, Q);
  }

  return true;

}
