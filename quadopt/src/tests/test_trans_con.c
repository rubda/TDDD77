#include <trans_con.h>
#include <assert.h>
#include <matLib.h>

void test_equality_constraints_transform();
void test_big_equality_constraints();

int main(){
  test_equality_constraints_transform();
  test_big_equality_constraints();

  return 0;
}

void test_equality_constraints_transform(){
  matrix* A = create_matrix(2, 2);
  value a_arr[4] = {5, 2, 4, 3};
  insert_array(a_arr, A);

  matrix* B = create_matrix(2, 1);
  value b_arr[4] = {7, 9};
  insert_array(b_arr, B);

  matrix* k = create_matrix(2, 1);
  value k_arr[2] = {1, 2};
  insert_array(k_arr, k);

  matrix* E = create_zero_matrix(6, 8);
  matrix* h = create_zero_matrix(6, 1);

  matrix* expected_h = create_matrix(6, 1);
  value eh_arr[6] = {1, 2, 0, 0, 0, 0};
  insert_array(eh_arr, expected_h);

  matrix* expected_E = create_matrix(6, 8);
  value eE_arr[48] = {1, 0, 0, 0, 0, 0, 0, 0,
		     0, 1, 0, 0, 0, 0, 0, 0,
		     -5, -2, 1, 0, 0, 0, 7, 0,
		     -4, -3, 0, 1, 0, 0, 9, 0,
		     0, 0, -5, -2, 1, 0, 0, 7,
		     0, 0, -4, -3, 0, 1, 0, 9};
  insert_array(eE_arr, expected_E);

  assert(dyn_2_eq(A, B, k, E, h, 2, 1));
  assert(compare_matrices(E, expected_E));
  assert(compare_matrices(h, expected_h));

  free_matrix(A);
  free_matrix(B);
  free_matrix(k);
  free_matrix(E);
  free_matrix(h);
  free_matrix(expected_E);
  free_matrix(expected_h);
}

void test_big_equality_constraints(){
  size_t card_x = 2;
  size_t card_u = 1;
  size_t N = 30;

  matrix* A = create_matrix(card_x, card_x);
  value a_arr[4] = {0.9721, 0.0155,
				  0.2114, 0.9705};
  insert_array(a_arr, A);

  matrix* B = create_matrix(card_x, card_u);
  value b_arr[2] = {0.0143, 0.6860};
  insert_array(b_arr, B);

  matrix* k = create_matrix(card_x, 1);
  value k_arr[2] = {15, 45};
  insert_array(k_arr, k);

  matrix* E = create_zero_matrix(card_x*(N + 1), N*(card_x + card_u) + card_x);
  matrix* h = create_zero_matrix(card_x*(N + 1), 1);

  assert(dyn_2_eq(A, B, k, E, h, card_x, card_u));

  free_matrix(A);
  free_matrix(B);
  free_matrix(k);
  free_matrix(E);
  free_matrix(h);
}
