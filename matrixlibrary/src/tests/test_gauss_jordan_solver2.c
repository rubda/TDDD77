#include <matLib.h>
#include <assert.h>
#include <stdio.h>

int main(void){

  matrix* A = create_matrix(3, 3);
  value temp_a[9] = { 0, 0, 1,
		      0, 1, 0,
		      1, 0, 0};
  insert_array(temp_a, A);

  matrix* B = create_matrix(3, 1);
  value temp_b[3] = { 0,
		      4,
		      3};
  insert_array(temp_b, B);

  matrix* X = create_matrix(3, 1);

  gauss_jordan_solver(A, X, B);

  /* X should be */
  matrix* solution = create_matrix(3, 1);
  value temp_solution[3] = {3,
			    4,
			    0};
  insert_array(temp_solution, solution);

  assert(compare_matrices(X, solution));

  free_matrix(A);
  free_matrix(B);
  free_matrix(X);
  free_matrix(solution);

  return 0;
}
