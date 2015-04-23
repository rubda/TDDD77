#include <matLib.h>
#include <assert.h>

void test_add(){
  matrix* a = create_matrix(2, 2);
  value a_values[4] = {2, 0, 1, 2};
  assert(insert_array(a_values, a));

  matrix* b = create_matrix(2, 2);
  value b_values[4] = {-1, -1, 1, 5};
  assert(insert_array(b_values, b));

  matrix* ref = create_matrix(2, 2);
  value ref_values[4] = {1, -1, 2, 7};
  assert(insert_array(ref_values, ref));

  matrix* c = create_matrix(2, 2);
  assert(add_matrices(a, b, c));
  assert(compare_matrices(c, ref));

  free_matrix(a);
  free_matrix(b);
  free_matrix(c);
  free_matrix(ref);
}

void test_sub(){
  matrix* a = create_matrix(2, 2);
  value a_values[4] = {2, 0, 1, 2};
  assert(insert_array(a_values, a));

  matrix* b = create_matrix(2, 2);
  value b_values[4] = {-1, -1, 1, 5};
  assert(insert_array(b_values, b));

  matrix* ref = create_matrix(2, 2);
  value ref_values[4] = {3, 1, 0, -3};
  assert(insert_array(ref_values, ref));

  matrix* c = create_matrix(2, 2);
  assert(subtract_matrices(a, b, c));
  assert(compare_matrices(c, ref));

  free_matrix(a);
  free_matrix(b);
  free_matrix(c);
  free_matrix(ref);
}

void test_multiply(){
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
  assert(multiply_matrices(a, b, c));
  assert(compare_matrices(c, ref));

  free_matrix(a);
  free_matrix(b);
  free_matrix(c);
  free_matrix(ref);
}

void test_vector_product(){
  matrix* a = create_matrix(2, 1);
  value a_values[2] = {2, 5};
  assert(insert_array(a_values, a));

  matrix* b = create_matrix(2, 1);
  value b_values[2] = {-1, 3};
  assert(insert_array(b_values, b));

  value val = dot_product(a, b);
  value ref = 13;
  assert(val == ref);

  free_matrix(a);
  free_matrix(b);
}

void test_add_rows(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {1, 2, 3,
		       4, 5, 6,
		       7, 8, 9};
  assert(insert_array(m_values, m));
  
  add_rows(1, 2, m);

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {1, 2, 3,
			 5, 7, 9,
			 7, 8, 9};
  assert(insert_array(ref_values, ref));

  assert(compare_matrices(m, ref));

  free_matrix(m);
  free_matrix(ref);
}

void test_sum_of_row(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {1, 2, 3,
		       4, 5, 6,
		       7, 8, 9};
  assert(insert_array(m_values, m));

  assert(sum_of_row(1, m) == 6);

  free_matrix(m);
}

void test_sum_of_col(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {1, 2, 3,
		       4, 5, 6,
		       7, 8, 9};
  assert(insert_array(m_values, m));

  assert(sum_of_column(1, m) == 12);

  free_matrix(m);
}

void test_multiply_matrix_with_scalar(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {1, 2, 3,
		       4, 5, 6,
		       7, 8, 9};
  assert(insert_array(m_values, m));

  multiply_matrix_with_scalar(-1, m);

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {-1, -2, -3,
			 -4, -5, -6,
			 -7, -8, -9};
  assert(insert_array(ref_values, ref));

  assert(compare_matrices(m, ref));

  free_matrix(m);
  free_matrix(ref);
}

void test_divide_matrix_with_scalar(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {2, 4, 6,
		       8, 10, 12,
		       14, 16, 18};
  assert(insert_array(m_values, m));

  divide_matrix_with_scalar(2, m);

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {1, 2, 3,
			 4, 5, 6,
			 7, 8, 9};
  assert(insert_array(ref_values, ref));

  assert(compare_matrices(m, ref));

  free_matrix(m);
  free_matrix(ref);
}

void test_multiply_row_with_scalar(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {1, 2, 3,
		       4, 5, 6,
		       7, 8, 9};
  assert(insert_array(m_values, m));

  multiply_row_with_scalar(-1, 1, m);

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {-1, -2, -3,
			 4, 5, 6,
			 7, 8, 9};
  assert(insert_array(ref_values, ref));

  assert(compare_matrices(m, ref));

  free_matrix(m);
  free_matrix(ref);
}

void test_divide_row_with_scalar(){
  matrix* m = create_matrix(3, 3);
  value m_values[9] = {2, 4, 6,
		       4, 5, 6,
		       7, 8, 9};
  assert(insert_array(m_values, m));

  divide_row_with_scalar(-2, 1, m);

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {-1, -2, -3,
			 4, 5, 6,
			 7, 8, 9};
  assert(insert_array(ref_values, ref));

  assert(compare_matrices(m, ref));

  free_matrix(m);
  free_matrix(ref);
}

void test_multiply_column_with_scalar(){}

void test_divide_column_with_scalar(){}

void test_transpose_matrix(){}

int main(){
  test_add();
  test_sub();
  test_multiply();
  test_vector_product();
  test_add_rows();
  test_sum_of_row();
  test_sum_of_col();
  test_multiply_matrix_with_scalar();
  test_divide_matrix_with_scalar();
  test_multiply_row_with_scalar();
  test_divide_row_with_scalar();
  test_multiply_column_with_scalar();
  test_divide_column_with_scalar();
  test_transpose_matrix();

  return 0;
}
