/*
  Author: Ruben Das
  Email: rubendas.rd@@gmail.com
  Date: 2015-03-12
  Description: This file tests the find_lagrange-function which is used in the solver.
*/

#include <matLib.h>
#include <work_set.h>
#include <find_lagrange.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

int main(void){
  clock_t begin, end;
  double time_spent;
  begin = clock();

  /* Creates A matrix */
  matrix* A = create_matrix(5, 2);
  value temp_a[10] = { 1, -2,
		      -1, -2,
		      -1,  2,
		       1,  0,
		       0,  1};
  insert_array(temp_a, A);

  /* Creates g matrix */
  matrix* g = create_matrix(2, 1);
  value temp_g[2] = {2, -5};
  insert_array(temp_g, g);

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

  /* Create lagrange vector */
  matrix* lagrange = create_matrix(A->rows, 1);
  value temp_lagrange[5] = {0, 0, 0, 0, 0};
  insert_array(temp_lagrange, lagrange);

  /* Test without prints... */

  for (int i = 1; i <= A->rows; i++){
    assert(get_value_without_check(i, 1, lagrange) == 0);
  }

  /* Iteration 1 */
  assert(w->count == 2);
  assert(w->data[0] == 3);
  assert(w->data[1] == 5);

  find_lagrange(g, A, d, z, w, lagrange);

  assert(w->count == 1);
  assert(w->data[0] == 5);

  assert(get_value_without_check(3, 1, lagrange) == -2);
  assert(get_value_without_check(5, 1, lagrange) == -1);

  /* Iteration 2 */
  value iter2_g[2] = {0, -5}; 
  insert_array(iter2_g, g);

  assert(w->count == 1);
  assert(w->data[0] == 5);
 
  find_lagrange(g, A, d, z, w, lagrange);

  assert(w->count == 0);

  assert(get_value_without_check(3, 1, lagrange) == -2);
  assert(get_value_without_check(5, 1, lagrange) == -5);

  /* Iteration 3 */
  work_set_append(w, 1);
  value iter3_g[2] = {0.8, -1.6};
  insert_array(iter3_g, g);

  assert(w->count == 1);
  assert(w->data[0] == 1);

  find_lagrange(g, A, d, z, w, lagrange);

  assert(w->count == 10);

  assert(abs(get_value_without_check(1, 1, lagrange) - 0.8) < 0.001);
  assert(get_value_without_check(3, 1, lagrange) == -2);
  assert(get_value_without_check(5, 1, lagrange) == -5);
  
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);

  return 0;
}

