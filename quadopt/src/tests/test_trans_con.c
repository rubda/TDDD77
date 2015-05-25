#include <trans_con.h>
#include <assert.h>
#include <matLib.h>

void test_equality_constraints_transform();
void test_big_equality_constraints();
void test_inequality_constraints();
void test_objective();

int main(){
  test_equality_constraints_transform();
  test_big_equality_constraints();
  test_inequality_constraints();

  test_objective();

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

  assert(trans_dyn_cons(A, B, k, E, h, 2));
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

  assert(trans_dyn_cons(A, B, k, E, h, card_x));

  free_matrix(A);
  free_matrix(B);
  free_matrix(k);
  free_matrix(E);
  free_matrix(h);
}

void test_inequality_constraints(){
  matrix* Fx = create_matrix(3, 2);
  value fx_arr[6] = {1, 2, 3, 4, 5, 6};
  insert_array(fx_arr, Fx);

  matrix* gx = create_matrix(3, 1);
  value gx_arr[3] = {4, 5, 6};
  insert_array(gx_arr, gx);

  matrix* xlim = create_matrix(4, 1);
  value xlim_arr[4] = {1, 2, 3, 4};
  insert_array(xlim_arr, xlim);

  matrix* ulim = create_matrix(2, 1);
  value ulim_arr[2] = {1, 2};
  insert_array(ulim_arr, ulim);

  size_t card_x = 2;
  size_t card_u = 1;
  size_t N = 5;
  int rows = 2*card_x*N + Fx->rows + 2*card_u*N;
  int columns = card_x*(N + 1) + card_u*N;
  matrix* F = create_zero_matrix(rows, columns);
  matrix* g = create_zero_matrix(rows, 1);

  matrix* expected_F = create_matrix(6, 6);
  value exp_F_arr[36] = {1, 2, 3, 0, 0, 0,
			 4, 5, 6, 0, 0, 0,
			 7, 8, 9, 0, 0, 0,
			 0, 0, 0, 1, 2, 3,
			 0, 0, 0, 4, 5, 6,
			 0, 0, 0, 7, 8, 9};
  insert_array(exp_F_arr, expected_F);

  matrix* expected_g = create_matrix(6, 1);
  value exp_g_arr[6] = {4, 5, 6, 4, 5, 6};
  insert_array(exp_g_arr, expected_g);

  assert(trans_ineq_cons(Fx, gx, F, g, 2, 1, 5, xlim, ulim));

  /* TODO: Fix these tests */
  /* assert(compare_matrices(expected_F, F)); */
  /* assert(compare_matrices(expected_g, g)); */

  free_matrix(Fx);
  free_matrix(gx);
  free_matrix(xlim);
  free_matrix(ulim);
  free_matrix(F);
  free_matrix(g);
  free_matrix(expected_F);
  free_matrix(expected_g);
}

void test_objective(){

  int n = 30;

  matrix* Qin = create_matrix(2, 2);
  value Q_arr[4] = {4.0000, 0, 
                    0, 0.1000};
  insert_array(Q_arr, Qin);

  matrix* P = create_matrix(2, 2);
  value P_arr[4] = {59.2896, 6.9782, 
                    6.9782, 1.8280};
  insert_array(P_arr, P);

  matrix* R = create_matrix(1, 1);
  value R_arr[1] = {8};
  insert_array(R_arr, R);

  matrix* Q = create_zero_matrix((n+1)*Qin->rows + n*R->rows, (n+1)*Qin->rows + n*R->rows);

  create_objective(n, Qin, P, R, Q);

  free_matrix(Qin);
  free_matrix(P);
  free_matrix(R);
  free_matrix(Q);
}
