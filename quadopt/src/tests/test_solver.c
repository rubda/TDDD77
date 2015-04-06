#include "solver.h"
#include "matLib.h"


/* example 16.3 from the book */

int main() {


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
  value z_arr[2] = {2, 
		     0};
  insert_array(z_arr, z);

  /* end point */
  matrix* z_end = create_matrix(2,1);
  insert_array(z_arr, z_end);

  
  create_problem(Q,q,NULL,NULL,F,g,z);


  printf("starting point: \n");
  print_matrix(z);

  /*z_end = quadopt_solver(z, G, d, A, b, 0);
*/
  printf("solution point: \n");
  print_matrix(z_end);



  return 0;


}
