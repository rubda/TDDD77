#include <matLib.h>
#include <assert.h>

int main(){
#ifndef INT
  matrix* a = create_matrix(4, 4);
  value a_values[16] = {
      -0.916300, -0.196700 ,-0.012300, 0.675200,

      1.787600 ,-0.890100, 0.072800, 0.786000,

      -0.820400, 0.910700, 0.939400, -2.132700,

      0.000000, 0.000000, 0.000000, 1.000000

  };
  assert(insert_array(a_values, a));
  matrix* b = create_matrix(4, 1);
  value b_values[4] = {
      0.219900,

      0.966300 ,

      0.350000 ,

      0.000000
  };
  assert(insert_array(b_values, b));
  matrix* x = create_matrix(4, 1);
  assert(gauss_jordan_solver(a,x,b));
  matrix* temp=multiply_matrices_with_return(a,x);
  /* print_matrix(a); */
  /* print_matrix(x); */
  /* print_matrix(temp); */
  /* print_matrix(b); */

  free_matrix(a);
  free_matrix(b);
  free_matrix(x);
  free_matrix(temp);
#endif /* !INT */
  return 0;
}
