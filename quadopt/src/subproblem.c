#include "subproblem.h"
#include <stdio.h>
#include <assert.h>

void solve_supbroblem(matrix* Ain, matrix* G, matrix* gk, matrix* d, matrix* z, matrix* p, matrix* lagrange, work_set* ws) {
  
  if (ws->count == 0){
    printf("This shit shouldn't happen'");
  }

  /* Create active conditions matrix */
  matrix* A = create_matrix(ws->count, Ain->columns);
  matrix* tmp_row = create_matrix(1, Ain->columns);

  int i;
  for (i = 0; i < ws->count; i++){
    get_row_vector(ws->data[i], Ain, tmp_row);
    insert_row_vector(i+1, tmp_row, A);
  }
  
  /* Create zero column */
  matrix* solved = create_matrix(Ain->columns, 1);
  matrix* zero_column = create_matrix(ws->count, 1);
  free(zero_column->start);
  zero_column->start = calloc(ws->count, sizeof(value)); 

  //print_matrix(A);
  //print_matrix(zero_column);

  /* Solve active conditions matrix = 0 */
  if (!solve_linear(A, solved, zero_column)) {
    printf("Should not enter here!\n");
  }

  //print_matrix(solved);

  free(A);
  free(tmp_row);
  free(solved);
  free(zero_column);
  

}
