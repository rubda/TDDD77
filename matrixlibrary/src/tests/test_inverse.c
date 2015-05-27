#include "matLib.h"
#include <assert.h>
#include <time.h>
#include <math.h>

/*make sure typedef float value*/

#ifdef FLOAT
value determinant_ref = -7329227.000000;
#endif /* FLOAT */
#ifndef FLOAT
value determinant_ref = -7329228.000000;
#endif /* !FLOAT */

int main(void) {
#ifndef INT
  /* This test is not valid for only integers. */
  clock_t begin, end;
  double time_spent;
  begin = clock();
  matrix* a = create_matrix(4, 4);
  value temp_a[16] = { 18, 60, 57, 96, 41, 24, 99, 58, 14, 30, 97, 66, 51, 13,
      19, 85, };
  insert_array(temp_a, a);
  value temp = get_determinant(a);
  assert(compare_elements(temp, determinant_ref) == 0);
  matrix* d = create_matrix(4, 4);
  get_inverse(a, d);
  matrix* c = create_matrix(4, 4);
  multiply_matrices(d, a, c);
  free_matrix(a);
  free_matrix(d);
  matrix* e = create_matrix(4, 4);
  value temp_e[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, };
  insert_array(temp_e, e);
  subtract_matrices(c, e, e);
  free_matrix(c);
  for (size_t i = 1; i <= e->rows; i++) {
    for (size_t j = 1; j <= e->columns; j++) {
      assert(abs(get_value(i,j,e))<0.00001);
    }
  }
  free_matrix(e);
  end = clock();
  time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);
#endif /* !INT */
  return 0;
}
