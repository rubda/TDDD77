#include <solver.h>
#include <matLib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

/* example 16.3 from the book */

int main(){
  clock_t begin, end;
  double time_spent;
  begin = clock();

  matrix* Q = create_matrix(2,2);
  value Q_arr[4] = {	2, 0,
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

  /* starting point */
  matrix* z = create_matrix(2,1);
  value z_arr[2] = {1, 
		    1};
  insert_array(z_arr, z);


  problem* problem = create_problem(Q, q, NULL, NULL, F, g, z, 0);

  quadopt_solver(problem);

  matrix* expected = create_matrix(2, 1);
  value e_arr[2] = {1.4,
		    1.7};
  insert_array(e_arr, expected);

  assert(compare_matrices(problem->solution, expected));

  free_matrix(expected);
  free_problem(problem);

  end = clock(); 
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);

  return 0;
}
