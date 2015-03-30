#include <stdio.h>
#include <work_set.h>
#include <assert.h>
#include <time.h>

int main() {
  clock_t begin, end;
  double time_spent;
  begin = clock();

  work_set* ws = work_set_create(5);
	
  work_set_append(ws, 7);
  work_set_append(ws, 8);
  work_set_append(ws, 1);
  work_set_append(ws, 3);
 
  assert(ws->data[0] == 7);
  assert(ws->data[1] == 8);
  assert(ws->data[2] == 1);
  assert(ws->data[3] == 3);

  work_set_remove(ws, 7);
  work_set_remove(ws, 3);

  assert(ws->count == 2);

  assert(work_set_contains(ws, 8));
  assert(work_set_contains(ws, 1));

  assert(work_set_free(ws));

  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time taken was: %f \n", time_spent);

  return 0;
}
