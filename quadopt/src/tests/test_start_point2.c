#include <solver.h>
#include <assert.h>
#include <feasible_point.h>

int main() {

  matrix* Q = create_matrix(4,4);
  value Q_arr[4] = {	2, 0,
			0, 2};
  insert_array(Q_arr, Q);

  matrix* q = create_matrix(4, 1);

  matrix* E = create_matrix(1, 4);
  value E_arr[4] = {1,   -1, 3, 8};
  insert_array(E_arr, E);


  matrix* h = create_matrix(1, 1);
  value h_arr[1] = {5};
  insert_array(h_arr, h);


  problem* problem = create_problem(Q, q, E, h, NULL, NULL, NULL, 0, 0);


  /*matrix* A = create_matrix(4,4);
  value A_arr[16] = {1,-1,3,8,
                      0,1,0,0,
                      0,0,0,1,
                      0,0,1,0};
  insert_array(A_arr, A);

  matrix* b = create_matrix(4,1);
  value b_arr[16] = {5,0,0,0};
  insert_array(b_arr, b);

  matrix* z = create_matrix(4,1);
  value z_arr[16] = {1,1,1,1};
  insert_array(z_arr, z);

  solve_linear(A,z,b);

  print_matrix(z);

  least_square(A,z,b);

  print_matrix(z);*/


  //assert(find_starting_point(problem));
  //assert(is_feasible_point(problem->z0, problem));

  free_problem(problem);
}
