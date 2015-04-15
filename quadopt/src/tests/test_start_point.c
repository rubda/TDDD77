#include <solver.h>
#include <assert.h>

int main() {

  matrix* Q = create_matrix(2,2);
  value Q_arr[4] = {	2, 0,
			0, 2};
  insert_array(Q_arr, Q);

  matrix* q = create_matrix(2, 1);
  value q_arr[2] = {  -20, 
		      -12};
  insert_array(q_arr, q);

  matrix* E = create_matrix(1, 2);
  value E_arr[2] = {1,   -1};
  insert_array(E_arr, E);


  matrix* h = create_matrix(1, 1);
  value h_arr[1] = {0};
  insert_array(h_arr, h);


  matrix* F = create_matrix(3, 2);
  value F_arr[6] = {  0.5, 1, 
		       1, 0,
		       0,  1};
  insert_array(F_arr, F);


  matrix* g = create_matrix(3, 1);
  value g_arr[3] = { 8, 
		     0, 
		     0};
  insert_array(g_arr, g);

  problem* problem = create_problem(Q, q, E, h, F, g, NULL);

  assert(find_starting_point(problem));

  assert(is_feasible_point(problem->z0, problem));

}
