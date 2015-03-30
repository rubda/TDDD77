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

  assert(abs(get_value_without_check(1, 1, X) + 1) < 0.0001);
  assert(abs(get_value_without_check(2, 1, X) - 1) < 0.0001);
  assert(abs(get_value_without_check(3, 1, X) - 1) < 0.0001);

  /*
  matrix* C = create_matrix(3, 2);
  value temp_c[6] = { 0,  1,
		      2,  0,
		      0,  2};
  insert_array(temp_c, C);


  matrix* D = create_matrix(3, 1);
  value temp_d[3] = { 0,
		     -2,
		      5};
  insert_array(temp_d, D);

  matrix* solved = create_matrix(2, 1);

  print_matrix(C);

  print_matrix(D);
  
  if (!solve_linear(C, solved, D)){
    printf("Can't be solved, using least_square.\n");
    least_square(C, solved, D);
  }

  print_matrix(solved);
  */

  return 0;
}
