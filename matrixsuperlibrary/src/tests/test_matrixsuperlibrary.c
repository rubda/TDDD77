/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-31
  Description: This is the matSuperLib-library testfile
*/

#include <matSuperLib.h>

int main(){
  matrix* temp=create_matrix(4,4);
  value temp_array[16]={1,1,1,2,0,1,3,1,0,0,1,6,0,0,0,1};
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

  free_matrix_m(temp1);
  free_matrix(temp);
  free_matrix_m(temp2);
  free_matrix_m(temp3);
  free_matrix_m(temp4);
  return 0;
}
