#include <matLib.h>
#include <work_set.h>
#include <find_lagrange.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

int main(void) {
  clock_t begin, end;
  double time_spent;
  begin = clock();

  /* Creates A matrix */
  matrix* A = create_matrix(5, 2);
  value temp_a[10] = {1, -2,
		      -1, -2,
		      -1,  2,
		      1,  0,
		      0,  1};
  insert_array(temp_a, A);

  /* Creates g matrix */
  matrix* g = create_matrix(2, 1);
  value temp_q[2] = {2, -5};
  insert_array(temp_q, g);

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

  /* create lagrange vector */
  matrix* lagrange = create_matrix(A->rows, 1);

  printf("----------------Iteration 1:------------------- \n");
  work_set_print(w);

  printf("Lagrange before: \n");
  print_matrix(lagrange);
  find_lagrange(g, A, d, z, w, lagrange);

  work_set_print(w);

  printf("Lagrange after: \n");
  print_matrix(lagrange);

  printf("----------------Iteration 2:------------------- \n");
  printf("Lagrange before: \n");
  print_matrix(lagrange);
  find_lagrange(g, A, d, z, w, lagrange);

  work_set_print(w);

  printf("Lagrange after: \n");
  print_matrix(lagrange);


  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);

  return 0;
}

