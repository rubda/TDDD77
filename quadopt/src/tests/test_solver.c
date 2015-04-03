#include <solver.h>
#include <matLib.h>


/* example 16.3 from the book */

int main() {


  matrix* G = create_matrix(2,2);
  value G_arr[4] = {	2, 0,
			0, 2};
  insert_array(G_arr, G);


  matrix* d = create_matrix(2, 1);
  value d_arr[2] = {  -2, 
		      -5};
  insert_array(d_arr, d);


  matrix* A = create_matrix(5, 2);
  value A_arr[10] = {  1, -2, 
		       -1, -2,
		       -1,  2, 
		       1,  0,
		       0,  1};
  insert_array(A_arr, A);


  matrix* b = create_matrix(5, 1);
  value b_arr[5] = { -2, 
		     -6, 
		     -2, 
		     0,
		     0};
  insert_array(b_arr, b);

  /* starting point */
  matrix* z = create_matrix(2,1);
  value z_arr[2] = {2, 
		     0};
  insert_array(z_arr, z);

  /* end point */
  matrix* z_end = create_matrix(2,1);
  insert_array(z_arr, z_end);

  printf("starting point: \n");
  print_matrix(z);

  /*z_end = quadopt_solver(z, G, d, A, b, 0);
*/
  printf("solution point: \n");
  print_matrix(z_end);



  return 0;


}
