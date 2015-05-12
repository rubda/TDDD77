#include <solver.h>
#include <matLib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>


int main(){
  clock_t begin, end;
  double time_spent;
  begin = clock();

  matrix* Q = create_matrix(2,2);
  value Q_arr[4] = {	2, 0,
			0, 2};
  insert_array(Q_arr, Q);


  matrix* q = create_zero_matrix(2, 1);


  matrix* E = create_matrix(1, 2);
  value E_arr[2] = {1,   -1};
  insert_array(E_arr, E);


  matrix* h = create_matrix(1, 1);
  value h_arr[1] = {0};
  insert_array(h_arr, h);


  matrix* F = create_matrix(4, 2);
  value F_arr[8] = {1, 1,
          -1, -1,
		       1, 0,
		       0,  1};
  insert_array(F_arr, F);
  multiply_matrix_with_scalar(-1, F);

  matrix* g = create_matrix(4, 1);
  value g_arr[4] = { -3,
          6, 
		     -1, 
		     -1};
  insert_array(g_arr, g);
  multiply_matrix_with_scalar(-1, g);

  problem* problem = create_problem(Q,q,E,h,F,g,NULL, 0, 0);

  /*quadopt_solver(problem);

  matrix* expected = create_matrix(2, 1);
  value e_arr[2] = {-1.5,
		    -1.5};
  insert_array(e_arr, expected);

  assert(compare_matrices(problem->solution, expected));

  free_matrix(expected);*/
  free_problem(problem);

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);

  return 0;
}
