#include <solver.h>
#include <matLib.h>
#include <assert.h>

int main(){

  matrix* G = create_matrix(2,2);
  value G_arr[4] = { 2, 0,
		     0, 2};
  insert_array(G_arr, G);


  matrix* d = create_matrix(2, 1);
  value d_arr[2] = { -2, 
		     -5};
  insert_array(d_arr, d);


  matrix* A = create_matrix(5, 2);
  value A_arr[10] = { 1, -2, 
		     -1, -2,
		     -1,  2, 
		      1,  0,
		      0,  1};
  insert_array(A_arr, A);


  matrix* b = create_matrix(5, 1);
  value b_arr[5] = {-2, 
		    -6, 
		    -2, 
		     0,
		     0};
  insert_array(b_arr, b);

  /* Starting point */
  matrix *z = create_matrix(2, 1);
  value z_arr[2] = { -2,
		      0};
  insert_array(z_arr, z);

  /* End point */
  matrix* z_end = create_matrix(2, 1);
  value z_arr_end[2] = { 0,
			 0};
  insert_array(z_arr_end, z_end);

  assert(z_end->start[0] = 1.4);
  assert(z_end->start[1] = 1.7);
  
  return 0;
}
