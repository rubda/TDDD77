#include <solver.h>
#include <assert.h>
#include <simplex.h>

int main(){

  matrix* Q = create_matrix(4,4);
  value Q_arr[16] = {2, 0, 1, 1,
  			              0, 2, 0, 1,
                      1, 0, 2, 0,
                      1, 1, 0, 2};
  insert_array(Q_arr, Q);

  matrix* q = create_zero_matrix(4, 1);

  matrix* E = create_matrix(1, 4);
  value E_arr[4] = {1,   -1, 3, 8};
  insert_array(E_arr, E);


  matrix* h = create_matrix(1, 1);
  value h_arr[1] = {5};
  insert_array(h_arr, h);


  problem* problem = create_problem(Q, q, E, h, NULL, NULL, NULL, 0, 0);
  
  assert(simplex_phase_1(problem));
  assert(is_feasible_point(problem->z0, problem));

  free_problem(problem);
}
