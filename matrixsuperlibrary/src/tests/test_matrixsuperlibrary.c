#include <matSuperLib.h>

int main(){
  matrix* temp=create_matrix(2,2);
  value temp_array[4]={1,2,1,2};
  insert_array(temp_array,temp);
  matrix_with_matrices* temp1=create_matrix_with_matrices(2,2);
  insert_matrix(1,1,temp,temp1);
  insert_matrix(1,2,temp,temp1);
  insert_matrix(2,1,temp,temp1);
  insert_matrix(2,2,temp,temp1);
  print_matrix_with_matrices(temp1);

}
