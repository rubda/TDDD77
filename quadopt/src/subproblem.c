#include "subproblem.h"
#include <stdio.h>
#include <assert.h>


matrix* get_active_conditions(matrix* Ain, work_set* ws);
matrix* get_zero_matrix(int rows, int columns);


void solve_supbroblem(matrix* Ain, matrix* G, matrix* gk, matrix* d,
		      matrix* z, matrix* p, matrix* lagrange, work_set* ws) {
  matrix* A = get_active_conditions(Ain, ws);

  matrix* zero = get_zero_matrix(p->rows, p->columns);


  bool success;
  do{
    success = solve_linear(A, p, zero);

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

  /* Please continue here */

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
