#include <matLib.h>
#include <assert.h>

void test_get_value(){
  matrix* m = create_matrix(2, 2);
  value m_values[4] = {2, 5, 7, 8};

  assert(insert_array(m_values, m));

  assert(get_value(1, 1, m) == 2);
  assert(get_value(1, 2, m) == 5);
  assert(get_value(2, 1, m) == 7);
  assert(get_value(2, 2, m) == 8);
  
  free_matrix(m);
}

void test_get_value_without_check(){
  matrix* m = create_matrix(2, 2);
  value m_values[4] = {2, 5, 7, 8};

  assert(insert_array(m_values, m));

  assert(get_value_without_check(1, 1, m) == 2);
  assert(get_value_without_check(1, 2, m) == 5);
  assert(get_value_without_check(2, 1, m) == 7);
  assert(get_value_without_check(2, 2, m) == 8);

  free_matrix(m);
}

void test_compare_matrices(){
  matrix* m1 = create_matrix(2, 2);
  value m1_values[4] = {2, 5, 7, 8};
  assert(insert_array(m1_values, m1));

  matrix* m2 = create_matrix(2, 2);
  value m2_values[4] = {2, 5, 7, 8};
  assert(insert_array(m2_values, m2));

  matrix* m3 = create_matrix(2, 2);
  value m3_values[4] = {2, 5, 4, 8};
  assert(insert_array(m3_values, m3));

  assert(compare_matrices(m1, m2));
  assert(!compare_matrices(m1, m3));

  free_matrix(m1);
  free_matrix(m2);
  free_matrix(m3);
}

void test_is_matrix(){
  matrix* m1 = create_matrix(2, 2);
  value m1_values[4] = {2, 5, 7, 8};
  assert(insert_array(m1_values, m1));

  matrix* m2 = create_matrix(2, 2);
  value m2_values[4] = {2, 5, 7, 8};
  assert(insert_array(m2_values, m2));

  assert(is_matrix(m1, m1));
  assert(!is_matrix(m1, m2));

  free_matrix(m1);
  free_matrix(m2);
}

void test_is_zero_matrix(){
  matrix* col_vector = create_matrix(5, 1);
  value col_values[5] = {0, 0, 0, 0, 0};
  assert(insert_array(col_values, col_vector));

  matrix* row_vector = create_matrix(1, 5);
  value row_values[5] = {0, 0, 0, 0, 0};
  assert(insert_array(row_values, row_vector));

  matrix* not_zero_col = create_matrix(5, 1);
  value not_zero_values_col[5] = {0, -1, 0, 3, 0};
  assert(insert_array(not_zero_values_col, not_zero_col));

  matrix* not_zero_row = create_matrix(1, 5);
  value not_zero_values_row[5] = {0, -1, 0, 3, 0};
  assert(insert_array(not_zero_values_row, not_zero_row));

  assert(is_zero_matrix(col_vector));
  assert(is_zero_matrix(row_vector));
  assert(!is_zero_matrix(not_zero_col));
  assert(!is_zero_matrix(not_zero_row));
	 
  free_matrix(col_vector);
  free_matrix(row_vector);
  free_matrix(not_zero_col);
  free_matrix(not_zero_row);
}

void test_is_non_negative_matrix(){
  matrix* col_vector = create_matrix(5, 1);
  value col_values[5] = {0, 0, -1, 0, 0};
  assert(insert_array(col_values, col_vector));

  matrix* row_vector = create_matrix(1, 5);
  value row_values[5] = {0, -1, 0, 0, 0};
  assert(insert_array(row_values, row_vector));

  matrix* not_zero = create_matrix(5, 1);
  value not_zero_values[5] = {0, -1, 0, 3, 0};
  assert(insert_array(not_zero_values, not_zero));

  matrix* col_vector_pos = create_matrix(5, 1);
  value col_values_pos[5] = {0, 0, 0, 0, 0};
  assert(insert_array(col_values_pos, col_vector_pos));

  matrix* row_vector_pos = create_matrix(1, 5);
  value row_values_pos[5] = {0, 3, 4, 0, 0};
  assert(insert_array(row_values_pos, row_vector_pos));

  assert(!is_non_negative_matrix(col_vector));
  assert(!is_non_negative_matrix(row_vector));
  assert(!is_non_negative_matrix(not_zero));
  assert(is_non_negative_matrix(col_vector_pos));
  assert(is_non_negative_matrix(row_vector_pos));
	 
  free_matrix(col_vector);
  free_matrix(row_vector);
  free_matrix(not_zero);
  free_matrix(col_vector_pos);
  free_matrix(row_vector_pos);
}

void test_is_non_negative_diagonal_matrix(){
  matrix* m1 = create_matrix(3, 3);
  value m1_values[9] = {0, 0, 0,
			0, 0, 0, 
			0, 0, 0};
  assert(insert_array(m1_values, m1));

  matrix* m2 = create_matrix(3, 3);
  value m2_values[9] = {0, 0, 0,
			0, -1, 0, 
			0, 0, 0};
  assert(insert_array(m2_values, m2));

  matrix* m3 = create_matrix(3, 3);
  value m3_values[9] = {0, -1, 0,
			0, 1, 0, 
			1, 0, 0};
  assert(insert_array(m3_values, m3));

  assert(is_non_negative_diagonal_matrix(m1));
  assert(!is_non_negative_diagonal_matrix(m2));
  assert(is_non_negative_diagonal_matrix(m3));

  free_matrix(m1);
  free_matrix(m2);
  free_matrix(m3);
}

void test_get_row_vector(){
  matrix* mat = create_matrix(3, 3);
  value mat_values[9] = {1, 2, 3,
			 4, 5, 6, 
			 7, 8, 9};
  assert(insert_array(mat_values, mat));

  matrix* row = create_matrix(1, 3);

  matrix* ref = create_matrix(1, 3);
  value ref_values[3] = {4, 5, 6};
  assert(insert_array(ref_values, ref));

  assert(get_row_vector(2, mat, row));
  assert(compare_matrices(row, ref));
  
  free_matrix(mat);
  free_matrix(row);
  free_matrix(ref);
}

void test_get_column_vector(){
  matrix* mat = create_matrix(3, 3);
  value mat_values[9] = {1, 2, 3,
			 4, 5, 6, 
			 7, 8, 9};
  assert(insert_array(mat_values, mat));

  matrix* col = create_matrix(3, 1);

  matrix* ref = create_matrix(3, 1);
  value ref_values[3] = {2, 5, 8};
  assert(insert_array(ref_values, ref));

  assert(get_column_vector(2, mat, col));
  assert(compare_matrices(col, ref));
  
  free_matrix(mat);
  free_matrix(col);
  free_matrix(ref);
}

void test_insert_row_vector(){
  matrix* mat = create_matrix(3, 3);
  value mat_values[9] = {1, 2, 3,
			 4, 5, 6, 
			 7, 8, 9};
  assert(insert_array(mat_values, mat));

  matrix* row = create_matrix(1, 3);
  value row_values[3] = {22, 55, 88};
  assert(insert_array(row_values, row));

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {1, 2, 3,
			 22, 55, 88,
			 7, 8, 9};
  assert(insert_array(ref_values, ref));

  assert(insert_row_vector(2, row, mat));
  assert(compare_matrices(mat, ref));
  
  free_matrix(mat);
  free_matrix(row);
  free_matrix(ref);
}

void test_insert_column_vector(){
  matrix* mat = create_matrix(3, 3);
  value mat_values[9] = {1, 2, 3,
			 4, 5, 6, 
			 7, 8, 9};
  assert(insert_array(mat_values, mat));

  matrix* col = create_matrix(3, 1);
  value col_values[3] = {22, 55, 88};
  assert(insert_array(col_values, col));

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {1, 22, 3,
			 4, 55, 6,
			 7, 88, 9};
  assert(insert_array(ref_values, ref));

  assert(insert_column_vector(2, col, mat));
  assert(compare_matrices(mat, ref));
  
  free_matrix(mat);
  free_matrix(col);
  free_matrix(ref);
}

void test_switch_rows(){
  matrix* mat = create_matrix(3, 3);
  value mat_values[9] = {1, 2, 3,
			 4, 5, 6, 
			 7, 8, 9};
  assert(insert_array(mat_values, mat));

  matrix* ref = create_matrix(3, 3);
  value ref_values[9] = {4, 5, 6,
			 1, 2, 3,
			 7, 8, 9};
  assert(insert_array(ref_values, ref));

  assert(switch_rows(1, 2, mat));
  assert(compare_matrices(mat, ref));
  
  free_matrix(mat);
  free_matrix(ref);
}

void test_get_sub_matrix(){
  matrix* mat = create_matrix(3, 3);
  value mat_values[9] = {1, 2, 3,
			 4, 5, 6, 
			 7, 8, 9};
  assert(insert_array(mat_values, mat));

  matrix* sub1 = create_matrix(2, 3);

  matrix* ref1 = create_matrix(2, 3);
  value ref1_values[6] = {4, 5, 6,
			  7, 8, 9};
  assert(insert_array(ref1_values, ref1));

  assert(get_sub_matrix(2, 3, 1, 3, mat, sub1));
  assert(compare_matrices(sub1, ref1));

  free_matrix(sub1);
  free_matrix(ref1);


  matrix* sub2 = create_matrix(2, 2);

  matrix* ref2 = create_matrix(2, 2);
  value ref2_values[6] = {4, 5,
			  7, 8};
  assert(insert_array(ref2_values, ref2));

  assert(get_sub_matrix(2, 3, 1, 2, mat, sub2));
  assert(compare_matrices(sub2, ref2));

  free_matrix(sub2);
  free_matrix(ref2);


  matrix* sub3 = create_matrix(2, 2);

  matrix* ref3 = create_matrix(2, 2);
  value ref3_values[6] = {5, 6,
			  8, 9};
  assert(insert_array(ref3_values, ref3));

  assert(get_sub_matrix(2, 3, 2, 3, mat, sub3));
  assert(compare_matrices(sub3, ref3));

  free_matrix(sub3);
  free_matrix(ref3);


  matrix* sub4 = create_matrix(2, 2);

  matrix* ref4 = create_matrix(2, 2);
  value ref4_values[6] = {1, 2,
			  4, 5};
  assert(insert_array(ref4_values, ref4));

  assert(get_sub_matrix(1, 2, 1, 2, mat, sub4));
  assert(compare_matrices(sub4, ref4));

  free_matrix(sub4);
  free_matrix(ref4);

  
  free_matrix(mat);
}

int main(){
  test_get_value();
  test_get_value_without_check();
  test_compare_matrices();
  test_is_matrix();
  test_is_zero_matrix();
  test_is_non_negative_matrix();
  test_is_non_negative_diagonal_matrix();
  test_get_row_vector();
  test_get_column_vector();
  test_insert_row_vector();
  test_insert_column_vector();
  test_switch_rows();
  test_get_sub_matrix();

  return 0;
}
