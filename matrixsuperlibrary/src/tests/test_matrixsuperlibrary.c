#include <matSuperLib.h>

int main(){
  matrix* temp=create_matrix(2,2);
  value temp_array[4]={1,2,1,2};
  insert_array(temp_array,temp);
  matrix_m* temp1=create_matrix_m(2,2);
  insert_matrix(1,1,temp,temp1);
  insert_matrix(1,2,temp,temp1);
  insert_matrix(2,1,temp,temp1);
  insert_matrix(2,2,temp,temp1);
  print_matrix_m(temp1);
  matrix_m* temp2=add_matrices_m_with_return(temp1,temp1);
  print_matrix_m(temp2);
  matrix_m* temp3=subtract_matrices_m_with_return(temp1,temp1);
  print_matrix_m(temp3);
  matrix_m* temp4=multiply_matrices_m_with_return(temp1,temp1);
  print_matrix_m(temp4);
  return 0;
}
