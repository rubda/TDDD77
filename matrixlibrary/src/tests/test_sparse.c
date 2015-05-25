#include <sparse_lib.h>

int main()
{
  matrix* Q = create_matrix(4, 4);
  value Q_arr[16] = {	2, 0, 0, 0, 
			0, 2, 0, 0, 
			1, 0, 2, 0, 
			1, 0, 0, 2};
  insert_array(Q_arr, Q);

  matrix* x = create_matrix(4, 2);
  value x_arr[8] = {	2, 1, 
			0, 1, 
			0, 1,
			0, 1};
  insert_array(x_arr, x);

  //matrix* b = create_zero_matrix(4, 2);


  sparse_matrix* sparse_Q = create_sparse_matrix(Q, -1);

  sparse_matrix* sparse_Qt = transpose_sparse_matrix_with_return(sparse_Q);

  matrix* b = multiply_sparse_matrix_matrix(sparse_Q, x);

  //print_matrix(b);
  //print_sparse_matrix(sparse_Qt);

  free_matrix(Q);
  free_matrix(b);
  free_matrix(x);

  free_sparse_matrix(sparse_Q);
  free_sparse_matrix(sparse_Qt);

  return 0;
}
