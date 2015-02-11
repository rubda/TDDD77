/*
  Author: Alexander Yngve
  Date: 2015-02-11
  Description: Short description of why this file is needed.
*/

#include <stdio.h>

/* Short explanation of why this type is needed. */
struct struct_t{
  int* pointer;
  int number;
};

typedef struct struct_t struct_t;

/* Short explanation of why this function is needed. */
int main(){
  printf("Hej\n");

  struct struct_t data_1;
  data_1.number = 4;
  printf("%i\n", data_1.number);

  struct_t data_2 = {NULL, 5};
  printf("%i\n", data_2.number);

  return 0;
}
