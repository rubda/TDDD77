#include <solver.h>
#include <assert.h>
#include <feasible_point.h>
#include <simplex.h>

int main() {

  matrix* Q = create_matrix(2,2);
  value Q_arr[4] = {  2, 0,
      0, 2};
  insert_array(Q_arr, Q);


  matrix* q = create_matrix(2, 1);
  value q_arr[2] = {  -2, 
          -5};
  insert_array(q_arr, q);


  matrix* F = create_matrix(5, 2);
  value F_arr[10] = {  1, -2, 
           -1, -2,
           -1,  2, 
           1,  0,
           0,  1};
  insert_array(F_arr, F);


  matrix* g = create_matrix(5, 1);
  value g_arr[5] = { -2, 
         -6, 
         -2, 
         0,
         0};
  insert_array(g_arr, g);

  problem* problem = create_problem(Q, q, NULL, NULL, F, g, NULL, 0, 0);

  assert(simplex_phase_1(problem));
  assert(is_feasible_point(problem->z0, problem));

  free_problem(problem);
}
