#include "matLib.h"

int main(){
  matrix* a = create_matrix(4, 4);
  value a_values[16] = {
    18, 60, 57, 96,
    41, 24, 99, 58,
    14, 30, 97, 66,
    51, 13, 19, 85
  };
  insert_array(a_values, a);
  free_matrix(a);  
}
