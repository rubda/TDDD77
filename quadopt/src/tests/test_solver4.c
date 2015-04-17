#include <solver.h>
#include <matLib.h>
#include <assert.h>

/* example 16.3 from the book */

int main() {

  matrix* Q = create_matrix(3, 3);
  value Q_arr[9] = {6, 2, 1,
		    2, 5, 2,
		    1, 2, 4};
  insert_array(Q_arr, Q);


  matrix* q = create_matrix(3, 1);
  value q_arr[3] = {-8, 
		    -3,
		    -3};
  insert_array(q_arr, q);

  matrix* E = create_matrix(2, 3);
  value E_arr[6] = {1, 0, 1,
		    0, 1, 1};
  insert_array(E_arr, E);


  matrix* h = create_matrix(2, 1);
  value h_arr[2] = {3,
		    0};
  insert_array(h_arr, h);

  matrix* z = create_matrix(3,1);
  value z_arr[3] = {5, 
		    2,
		    -2};
  insert_array(z_arr, z);

  problem* problem = create_problem(Q, q, E, h, NULL, NULL, z);

  quadopt_solver(problem);

  matrix* expected = create_matrix(3, 1);
  value e_arr[3] = {2,
		    -1,
		    1};
  insert_array(e_arr, expected);

  assert(compare_matrices(problem->solution, expected));

  free_matrix(expected);
  free_problem(problem);

  return 0;
}
