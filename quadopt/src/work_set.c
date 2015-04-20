#include <stdlib.h>
#include <stdio.h>
#include <work_set.h>

/* Creates a new work set */
work_set* work_set_create(int ws_max) {
  /*TODO validate indata*/
  work_set* ws = (work_set*)malloc(sizeof(work_set));
  ws->data = malloc(ws_max*sizeof(int));
  ws->count = 0;
  return ws;
}

/* Adds an element to the set */
bool work_set_append(work_set* ws, int val) {
  /*TODO validate indata*/
  ws->data[ws->count] = val;
  ws->count++;
  return true;
}

/* Removes an element from the set */
bool work_set_remove(work_set* ws, int val) {
  /*TODO validate indata*/
  /*maybe want to keep order of conditions, but for now: mess up order on remove!*/
  int i;
  for (i = 0; i < ws->count; i++) {
    if (ws->data[i] == val) {
      if (i == ws->count-1) {
	ws->count--;
	return true;
      }else{
	ws->data[i] = ws->data[ws->count-1];
	ws->count--;
    	return true;
      }
    }
  }
  return false;
}

/* Removes and deallocates the set */
bool work_set_free(work_set* ws) {
  free(ws->data);
  ws->data = NULL;
  free(ws);
  ws = NULL;
  return true;
}

/* Prints all current elements in the set */
void work_set_print(work_set* ws) {
  printf("work_set: ");
  int i;
  for (i = 0; i < ws->count-1; i++) {
    printf("%d, ", ws->data[i]);
  }
  if (ws->count > 0) {
    printf("%d\n\n", ws->data[ws->count-1]);
  }
}

/* Checks if the set is containing the item */
bool work_set_contains(work_set* ws, int item) {
  int i;
  for (i = 0; i < ws->count; i++) {
    if (item == ws->data[i]) {
      return true;
    }
  }
  return false;
}

/* Clears the set */
void work_set_clear(work_set* ws) {
  ws->count = 0;
}
