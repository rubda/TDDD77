#include <matLib.h>
#include <assert.h>
#include <stdio.h>

int main(void){

  matrix* A = create_matrix(4, 3);
  value temp_a[12] = { 1,  0,  1,
		       0,  1, -1,
		       1,  1,  0,
		       0,  1,  1};
  insert_array(temp_a, A);

  matrix* B = create_matrix(4, 1);
  value temp_b[4] = { 1,
		      1,
		     -1,
		      2};
  insert_array(temp_b, B);

  matrix* X = create_matrix(3, 1);

  if (!solve_linear(A, X, B)){
    least_square(A, X, B);
  }

  assert(get_value_without_check(1, 1, X) == -1);
  assert(get_value_without_check(2, 1, X) == 1);
  assert(abs(get_value_without_check(3, 1, X) - 1) < 0.0001);
 
  //Denna assert borde också funka istället för den övre med abs...
  //assert(get_value_without_check(3, 1, X) == 1);

  return 0;
}
