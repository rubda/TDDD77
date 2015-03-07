#include <matLib.h>
#include <work_set.h>
#include <find_lagrange.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void) {
  clock_t begin, end;
  double time_spent;
  begin = clock();

  /* Creates A matrix */
  matrix* A = create_matrix(5, 2);
  value temp_a[10] = {
    1, -2,
   -1, -2,
   -1,  2,
    1,  0,
    0,  1,
  };
  insert_array(temp_a, A);

  /* Creates Q matrix */
  matrix* Q = create_matrix(2, 2);
  value temp_q[4] = {2, 0, 0, 2};
  insert_array(temp_q, Q);

  /* Creates d matrix */
  matrix* d = create_matrix(2, 1);
  value temp_d[2] = {-2, -5};
  insert_array(temp_d, d);

  /* Creates z matrix */
  matrix* z = create_matrix(2, 1);
  value temp_z[2] = {2, 0};
  insert_array(temp_z, z);

  /* Creates work set */
  work_set* w =  work_set_create(A->rows);
  work_set_append(w, 3);
  work_set_append(w, 5);

  work_set_print(w);

  find_lagrange(Q, A, d, z, w);

  printf("A= \n");
  print_matrix(A);

  printf("Q= \n");
  print_matrix(Q);
	
  printf("d= \n");
  print_matrix(d);

  printf("z= \n");
  print_matrix(z);

  work_set_print(w);

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);
}

