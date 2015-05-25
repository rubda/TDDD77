#include "matLib.h"
#include <assert.h>
#include <time.h>
#include <math.h>

/*make sure typedef float value*/

int main(void) {
  clock_t begin, end;
  double time_spent;
  begin = clock();
  int var=7;
  /* Creates A matrix */
  matrix* a = create_matrix(var, var);
  value temp_a[49] = {
    18, 60, 57, 96,
    41, 24, 99, 58,
    14, 30, 97, 66,
    51, 13, 19, 85,
    18, 60, 57, 96,
    41, 24, 99, 58,
    41, 24, 99, 58,
    41, 24, 99, 58,
    41, 24, 99, 58,
    41, 24, 99, 58,
    41, 24, 99, 58,
    41, 24, 99, 58,
    9.0
  };
  insert_array(temp_a, a);

  /* Creates b matrix */
  matrix* b = create_matrix(var, 1);
  value temp_b[7] = { 18.0, 60.0, 57.0, 96.0,18.0,9.0,5.0 };
  insert_array(temp_b, b);

  /* Creates x matrix */
  matrix* x = create_matrix(var, 1);

  solve_linear(a,x,b);

  /* printf("Ax=b \n"); */
  /* printf("A= \n"); */
  /* print_matrix(a); */
  /* printf("x= \n"); */
  /* print_matrix(x); */
  /* printf("b= \n"); */
  /* print_matrix(b); */

  matrix* b_new = create_matrix(var, 1);
  multiply_matrices(a,x,b_new);
  /* printf("Ax= \n"); */
  /* print_matrix(b_new); */
  matrix* diff = create_matrix(var, 1);
  subtract_matrices(b_new,b,diff);
  /* printf("difference is: \n"); */
  /* print_matrix(diff); */
  /* printf("size of value is %li \n",sizeof(value)); */

#ifndef INT
  /* Standard case for float, double and quad. */
  double tolerance = PRECISION;
#endif /* !INT */
#ifdef INT
  /* This test is invalid when only working with integers. */
  double tolerance = 100;
#endif /* INT */

  for(size_t i = 0; i < diff->rows; i++){
    double d = fabs((double)diff->start[i]);
   if(d > tolerance){
     printf("solve_linear failed! Difference between answer and expected is greater than %f: %f\n", tolerance, d);
      return 1;
    }
  }

  free_matrix(a);
  free_matrix(b);
  free_matrix(x);
  free_matrix(b_new);
  free_matrix(diff);

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n",time_spent);

  return 0;
}

