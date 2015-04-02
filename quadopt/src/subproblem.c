#include "subproblem.h"
#include <stdio.h>
#include <assert.h>


matrix* get_active_conditions(matrix* Ain, work_set* ws);
matrix* get_zero_matrix(int rows, int columns);


void solve_supbroblem(matrix* Ain, matrix* G, matrix* gk, matrix* d,
		      matrix* z, matrix* p, matrix* lagrange, work_set* ws) {

  if (ws->count == 0) {
    /* solve derivative and get vector pointing towards the global minimum */
    matrix* min = create_matrix(p->rows,p->columns);
    value sum, d_val;
    int r, c;

    print_matrix(G);
    print_matrix(gk);

    for (c = 1; c <= G->columns; c++) {
      sum = 0;
      for (r = 1; r <= G->rows; r++) {
        sum += get_value_without_check(r,c,G);
      }
      d_val = get_value_without_check(c,1,gk);
      insert_value_without_check((-d_val)/sum,c,1,p);
    }
/*
    matrix* zn = matrix_copy(z);
    multiply_matrix_with_scalar(-1,zn);
    add_matrices(min,zn,p);*/
    return;
  }


  /* solve system as long as you get the the zero vector */
  matrix* A = get_active_conditions(Ain, ws);
  matrix* zero = get_zero_matrix(p->rows, p->columns);

  printf("\n----- A -----\n");
  print_matrix(A);

  printf("\n----- z -----\n");
  print_matrix(z);


  bool success;
  do{
    success = gauss_jordan(A);

    if(!success){
      printf("Could not solve conditions to subproblem!\n");
    }else{
      printf("Solution from conditions in subproblem:\n");
      print_matrix(p);

      /* Remove condition */
      find_lagrange(gk, A, d, z, ws, lagrange);
      
      /* Resize A matrix */
      free_matrix(A);
      A = get_active_conditions(Ain, ws);
    }
  }while(success);

  /* use range space to get p */
  matrix* G_inv = matrix_copy(G);
  get_inverse(G,G_inv);

  printf("\n----- G_inv -----\n");
  print_matrix(G_inv);

/* deebug - find lagrange doesnt work */
  value A_arr[2] = {1,0};
  insert_array(A_arr,A);

  matrix* At = transpose_matrix_with_return(A);

  printf("\n----- A -----\n");
  print_matrix(A);

  printf("\n----- At -----\n");
  print_matrix(At);

  matrix* AG = create_matrix(A->rows,G_inv->columns);
  multiply_matrices(A,G_inv,AG);

  printf("\n----- AG -----\n");
  print_matrix(AG);

  matrix* AGAt = create_matrix(AG->rows,At->columns);
  multiply_matrices(AG,At,AGAt);

  printf("\n----- AGAt -----\n");
  print_matrix(AGAt);

  matrix* AGg = create_matrix(AG->rows,gk->columns);
  multiply_matrices(AG,gk,AGg);

  printf("\n----- AGg -----\n");
  print_matrix(AGg);

  matrix* Az = create_matrix(A->rows,z->columns);
  multiply_matrices(A,z,Az);

  printf("\n----- Az -----\n");
  print_matrix(Az);

  matrix* h1 = matrix_copy(AGg);
  subtract_matrices(AGg, Az, h1);

  printf("\n----- h1 -----\n");
  print_matrix(h1);

  matrix* lambda = matrix_copy(h1);
  solve_linear(AGAt,lambda,h1);

  printf("\n----- lambda -----\n");
  print_matrix(lambda);

  matrix* ht = create_matrix(p->rows,lambda->columns);
  matrix* h2 = matrix_copy(ht);
  multiply_matrices(At,lambda,ht);
  subtract_matrices(ht,gk,h2);

  printf("\n----- h2 -----\n");
  print_matrix(h2);


  solve_linear(G,p,h2);

  print_matrix(p);

  /*TODO FREE everything*/
  free_matrix(zero);
  free_matrix(A);
}

matrix* get_active_conditions(matrix* Ain, work_set* ws){
  matrix* A = create_matrix(ws->count, Ain->columns);
  
  bool success = false;
  for(int i = 0; i < ws->count; i++){
    matrix* temp_row = get_row_vector_with_return(ws->data[i], Ain);
    success = insert_row_vector(i + 1, temp_row, A);
    free_matrix(temp_row);
  }

  if(success){
    return A;
  }else{
    free_matrix(A);
    return NULL;
  }
}

matrix* get_zero_matrix(rows, columns){
  matrix* zero = create_matrix(rows, columns);
  free(zero->start);
  zero->start = calloc(rows * columns, sizeof(value));

  if(zero->start == NULL){
    return NULL;
  }

  return zero;
}
