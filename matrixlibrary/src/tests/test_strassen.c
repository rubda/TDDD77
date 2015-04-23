#include <matLib.h>
#include <assert.h>

int main(){
  matrix* a = create_matrix(2, 2);
  value a_values[4] = {2, 0, 1, 2};
  assert(insert_array(a_values, a));

  matrix* b = create_matrix(2, 2);
  value b_values[4] = {-1, -1, 1, 5};
  assert(insert_array(b_values, b));

  matrix* ref = create_matrix(2, 2);
  value ref_values[4] = {-2, -2, 1, 9};
  assert(insert_array(ref_values, ref));

  matrix* c = create_matrix(2, 2);
  assert(strassen_matrices(a, b, c));
  assert(compare_matrices(c, ref));

  free_matrix(a);
  free_matrix(b);
  free_matrix(c);
  free_matrix(ref);

  matrix* d = create_matrix(3, 3);
  value d_values[9] = {1,1,1,2,2,2,3,3,3};
  assert(insert_array(d_values, d));

  matrix* e = create_matrix(3, 3);
  value e_values[9] = {4,4,4,5,5,5,6,6,6};
  assert(insert_array(e_values, e));

  matrix* ref_new = create_matrix(3, 3);
  value ref_values_new[9] = {15, 15,15,30,30,30,45,45,45};
  assert(insert_array(ref_values_new, ref_new));

  matrix* f = create_matrix(3, 3);
  assert(strassen_matrices(d, e, f));
  assert(compare_matrices(f, ref_new));
  free_matrix(d);
  free_matrix(e);
  free_matrix(f);
  free_matrix(ref_new);

  return 0;
}
