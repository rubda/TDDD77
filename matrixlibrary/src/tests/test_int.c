#include "matLib.h"
#include <assert.h>
#include <time.h>
int main(void) {
  clock_t begin, end;
  double time_spent;
  begin = clock();

  matrix* a = create_matrix(4, 4);
  value temp_a[16] = { 18, 60, 57, 96,
		       41, 24, 99, 58,
		       14, 30, 97, 66,
		       51, 13, 19, 85 };
  insert_array(temp_a, a);

  matrix* b = create_matrix(4, 4);
  assert(insert_array(temp_a, b));


  //tests check_boundaries
  assert(check_boundaries(1,1,a));
  assert(check_boundaries(4,4,a));
  assert(!check_boundaries(4,5,a));
  assert(!check_boundaries(5,4,a));
  assert(!check_boundaries(0,1,a));
  assert(!check_boundaries(1,0,a));
  assert(!check_boundaries(-1,1,a));
  assert(!check_boundaries(1,-1,a));


  //tests compare_matrices,insert_value and get_value
  assert(compare_matrices(a,b));
  assert(insert_value(10,1,1,b));
  assert(!compare_matrices(a,b));
  assert(get_value(1,1,b)==10);
  assert(insert_value(18,1,1,b));
  assert(compare_matrices(a,b));


  //tests is_matrix
  matrix* c=a;
  assert(compare_matrices(a,c));
  assert(!is_matrix(a,b));
  assert(is_matrix(a,c));


  //tests insert_value by trying to go outside the matrix
  assert(insert_value(1,1,1,c));
  assert(insert_value(2,2,2,c));
  assert(insert_value(3,3,3,c));
  assert(insert_value(4,4,4,c));
  assert(!insert_value(5,5,5,c));
  assert(!insert_value(-1,-1,-1,c));
  assert(!insert_value(-1,-1,1,c));
  assert(!insert_value(-1,1,-1,c));

  //test get_value
  assert(get_value(1,1,c)==1);
  assert(get_value(2,2,c)==2);
  assert(get_value(3,3,c)==3);
  assert(get_value(4,4,c)==4);
  assert(get_value(0,0,c)==0);
  assert(get_value(1,-1,c)==0);
  assert(get_value(-1,1,c)==0);
  assert(get_value(5,5,c)==0);

  //tests insert and get without boundary checks
  insert_value_without_check(4,1,1,c);
  insert_value_without_check(3,2,2,c);
  insert_value_without_check(2,3,3,c);
  insert_value_without_check(1,4,4,c);
  assert(get_value_without_check(1,1,c)==4);
  assert(get_value_without_check(2,2,c)==3);
  assert(get_value_without_check(3,3,c)==2);
  assert(get_value_without_check(4,4,c)==1);

  //tests add_matrices
  value temp_b[16]={
    36,120,114,192,
    82,48,198,116,
    28, 60, 194,132,
    102,26,38,170};
  assert(insert_array(temp_b,a));
  matrix* d = create_matrix(4, 4);
  assert(add_matrices(b,b,d));
  assert(compare_matrices(d,a));

  //tests subtract_matrices
  value temp_c[16]={
    0,0,0,0,
    0,0,0,0,
    0, 0, 0,0,
    0,0,0,0};
  assert(insert_array(temp_c,a));
  assert(subtract_matrices(b,b,d));
  assert(compare_matrices(d,a));

  //tests sum_of_row
  assert(insert_array(temp_a,a));
  assert(sum_of_row(1,a)==231);
  assert(sum_of_row(4,a)==168);
  assert(sum_of_row(0,a)==0);
  assert(sum_of_row(5,a)==0);

  //tests sum_of_column
  assert(sum_of_column(1,a)==124);
  assert(sum_of_column(4,a)==305);
  assert(sum_of_column(0,a)==0);
  assert(sum_of_column(5,a)==0);

  //tests get_row_vector
  matrix* e = create_matrix(1, 4);
  value temp_d[4] = { 18, 60, 57, 96};
  assert(insert_array(temp_d,e));
  matrix* f = create_matrix(1, 4);
  assert(!get_row_vector(0,a,f));
  assert(!get_row_vector(5,a,f));
  assert(get_row_vector(1,a,f));
  assert(compare_matrices(e,f));

  //tests get_column_vector
  matrix* g = create_matrix(4, 1);
  assert(insert_array(temp_d,e));
  matrix* h = create_matrix(1, 4);
  assert(!get_row_vector(0,a,h));
  assert(!get_row_vector(5,a,h));
  assert(get_row_vector(1,a,h));
  assert(compare_matrices(e,h));

  //tests mulitply_matrices
  assert(multiply_matrices(a,a,b));
  value temp_f[16]={8478,5478,14319,17130,
		    6066,6760,15418,16792,
		    6206,5328,14431,15096,
		    6052,5047,7652,14129.00};
  assert(insert_array(temp_f,d));
  assert(compare_matrices(b,d));
  assert(!multiply_matrices(a,h,b));
  assert(!multiply_matrices(a,a,h));

  //tests transpose_matrix
  value temp_g[16]={18,41,14,51,
		    60,24,30,13,
		    57,99,97,19,
		    96,58,66,85};
  assert(insert_array(temp_g,d));
  assert(transpose_matrix(a,b));
  assert(compare_matrices(b,d));
  assert(!transpose_matrix(e,b));
  assert(!transpose_matrix(a,e));

  //tests multiply_matrix_with_scalar
  value temp_h[16] = { 36, 120, 114, 192,
		       82, 48, 198, 116,
		       28, 60, 194, 132,
		       102, 26, 38, 170 };
  assert(insert_array(temp_h,b));
  multiply_matrix_with_scalar(2,a);
  assert(compare_matrices(a,b));

  //test get_sub_matrix
  matrix* i=create_matrix(2,2);
  assert(insert_array(temp_a,a));
  assert(get_sub_matrix(1,2,1,2,a,i));
  matrix* j=create_matrix(2,2);
  value temp_i[4] = { 18, 60, 41, 24};
  assert(insert_array(temp_i,j));
  assert(compare_matrices(j,i));
  value temp_j[4] = { 97, 66, 19, 85};
  assert(insert_array(temp_j,j));
  assert(get_sub_matrix(3,4,3,4,a,i));
  assert(compare_matrices(j,i));
  assert(!get_sub_matrix(2,4,3,4,a,i));
  assert(!get_sub_matrix(3,4,2,4,a,i));
  assert(!get_sub_matrix(4,5,4,5,a,i));
  assert(!get_sub_matrix(0,1,0,1,a,i));

  //test insert_row_vector
  assert(insert_array(temp_a,a));
  value temp_k[16] = { 18, 60, 57, 96,
		       18, 60, 57, 96,
		       14, 30, 97, 66,
		       51, 13, 19, 85 };
  assert(insert_array(temp_k,b));
  assert(insert_array(temp_d,e));
  assert(insert_row_vector(2,e,a));
  assert(compare_matrices(a,b));

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n",time_spent);
  free_matrix(a);
  free_matrix(b);
  free_matrix(d);
  free_matrix(e);
  free_matrix(f);
  free_matrix(g);
  free_matrix(h);
  free_matrix(i);
  free_matrix(j);

  return 0;
}
