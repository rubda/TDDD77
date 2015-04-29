#include <trans_con.h>
#include <assert.h>
#include <matLib.h>

int main(){
  matrix* k = create_matrix(2, 1);
  value k_arr[2] = {1, 2};
  insert_array(k_arr, k);

  matrix* h = create_zero_matrix(5, 1);

  assert(dyn_2_eq(NULL, NULL, k, NULL, h));

  matrix* expected_h = create_matrix(5, 1);
  value eh_arr[5] = {1, 2, 0, 0, 0};
  insert_array(eh_arr, expected_h);

  assert(compare_matrices(h, expected_h));

  free_matrix(k);
  free_matrix(h);
  free_matrix(expected_h);

  return 0;
}
