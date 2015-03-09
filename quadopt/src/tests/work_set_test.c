#include <stdio.h>
#include <work_set.h>
#include <assert.h>

int main() {
  work_set* ws = work_set_create(5);

  printf("------------------\n");
	
  work_set_append(ws, 7);
  work_set_append(ws, 8);
  work_set_append(ws, 1);
  work_set_append(ws, 3);


  work_set_print(ws);
  printf("\n");
  work_set_remove(ws, 7);

  work_set_print(ws);

  printf("\n");
  return 0;
}
