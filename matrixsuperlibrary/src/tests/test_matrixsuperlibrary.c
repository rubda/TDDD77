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
  matrix* temp1 = create_matrix(4, 4);
  int column;
  print_matrix(temp);
  for (int i = 1; i <= temp->rows; i++) {
    column = first_nonezero_in_row_index(i, 1, temp);
    printf("%i \n", column);
    /* A zero row indicates that the variable is unbound */
    if (column == 0) {
      insert_value(1, i, i, temp1);
    }
    else {
      if (column + 1 > temp1->columns) {
        insert_value(1, i, i, temp1);
      }
      else {
        column = first_nonezero_in_row_index(i, column + 1, temp);
        /* If a row only contains one variable it must be zero */
        if (column == 0) {
          insert_value(0, i, i, temp1);
        }
        else {
          /* This is the motherfucker,  */
          for (int j = 1; j <= i; j++) {
            insert_value(0, i, j, temp1);
          }
          for (int j = i + 1; j <= temp->columns; j++) {
            insert_value(-get_value(i, j, temp), i, j, temp1);
          }

        }
      }
    }

  }
  print_matrix(temp1);
  return 0;
}
