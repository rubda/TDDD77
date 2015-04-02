#include <assert.h>
#include <subproblem.h>
#include <matLib.h>
#include <work_set.h>

int main(){
  /* Creates A matrix */
  matrix* A = create_matrix(5, 2);
  value temp_a[10] = { 1, -2,
		      -1, -2,
		      -1,  2,
		       1,  0,
		       0,  1};
  insert_array(temp_a, A);

  /* Create work set */
  work_set* ws = work_set_create(2);
  work_set_append(ws, 4);
  work_set_append(ws, 5);

  /* Creates p and lagrange matrix */
  matrix* p = create_matrix(A->columns, 1);
  matrix* lagrange = create_matrix(A->rows, 1);

  /* Create more matrix stuff */
  matrix* G = create_matrix(2,2);
  value G_arr[4] = {2, 0,
		    0, 2};
  insert_array(G_arr, G);

  matrix* gk = create_matrix(2, 1);
  value gk_arr[2] = {-2,
		     -5};
  insert_array(gk_arr, gk);

  matrix* d = create_matrix(2, 1);
  value d_arr[2] = {-2, 
		    -5};
  insert_array(d_arr, d);

  matrix* z = create_matrix(2,1);
  value z_arr[2] = {0, 
		    0};
  insert_array(z_arr, z);

  /* Solve subproblem */



  solve_supbroblem(A, G, gk, d, z, p, lagrange, ws);

  free_matrix(A);
  free_matrix(p);
  free_matrix(G);
  free_matrix(gk);
  free_matrix(d);
  free_matrix(z);
  free_matrix(lagrange);
  work_set_free(ws);

  return 0;
}
