#include <sparse_lib.h>

int main()
{
	matrix* Q = create_matrix(4, 4);
  value Q_arr[16] = {	2, 0, 0, 0, 
  										0, 2, 0, 0, 
  										1, 0, 2, 0, 
  										1, 0, 0, 2};
  insert_array(Q_arr, Q);

  print_matrix(Q);

  //sparse_matrix* sparse_Q = create_sparse_matrix(Q, -1);

  //printf("Size: %d \n", sparse_Q->size);

  //print_sparse_matrix(sparse_Q);

  free_matrix(Q);



	return 0;
}