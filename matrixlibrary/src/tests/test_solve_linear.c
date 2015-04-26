#include <matLib.h>
#include <assert.h>
#include <stdio.h>

int main(void){

  matrix* A = create_matrix(2, 2);
  value temp_a[4] = { 0, 1,
		      1, 0};
  insert_array(temp_a, A);

  matrix* B = create_matrix(2, 1);
  value temp_b[2] = { 5,
		      4};
  insert_array(temp_b, B);

  matrix* X = create_matrix(2, 1);

  solve_linear(A, X, B);
  
  /* X should be */
  matrix* solution = create_matrix(2, 1);
  value temp_solution[2] = {4,
			    5};
  insert_array(temp_solution, solution);

  printf("What solve_linear returns:\n");
  print_matrix(X);

  printf("What solve_linear should return:\n");
  print_matrix(solution);

  //assert(compare_matrices(X, solution));

  /* The problem is that the matrix does not pivot the matrix correct,
     before trying to solve the system Ax = b? D
  */

  free_matrix(A);
  free_matrix(B);
  free_matrix(X);
  free_matrix(solution);

  return 0;
}
