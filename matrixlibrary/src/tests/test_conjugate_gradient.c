#include <sparse_lib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

int main(){
  clock_t begin, end;
  double time_spent;
  begin = clock();

  matrix* Q = create_matrix(4, 4);
  value Q_arr[16] = {1, 0, 1, 0,
		     0, 2, 0, 1,
		     1, 0, 2, 0,
		     0, 1, 0, 1};
  insert_array(Q_arr, Q);

  sparse_matrix* s_Q = create_sparse_matrix(Q, 8);

  matrix* q = create_matrix(4, 1);
  value q_arr[4] = {3,
		    20,
		    5,
		    15};
  insert_array(q_arr, q);

  matrix* expected = create_matrix(4, 1);
  value e_arr[4] = {1,
                    5,
                    2,
                    10};
  insert_array(e_arr, expected);

  matrix* x = create_zero_matrix(4, 1);


  conjugate_gradient(s_Q, x, q);

  assert(compare_matrices(x, expected));

  free_matrix(Q);
  free_matrix(q);
  free_matrix(x);
  free_matrix(expected);
  free_sparse_matrix(s_Q);


  end = clock(); 
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);

}
