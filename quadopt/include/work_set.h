#ifndef WORK_SET_H
#define WORK_SET_H

#include <stdbool.h>

/* Structure for storing different sets */
struct work_set{
  int count;
  int* data;
};

typedef struct work_set work_set;

/* Creates a new work set */
work_set* work_set_create(int ws_max);

/* Removes and deallocates the set */
bool work_set_free(work_set* ws);

/* Adds an element to the set */
bool work_set_append(work_set* ws, int val);

/* Removes an element from the set */
bool work_set_remove(work_set* ws, int val);

/* Prints all current elements in the set */
void  work_set_print(work_set* ws);

/* Checks if the set is containing the item */
bool work_set_contains(work_set* ws, int item);

/* Clears the set */
void work_set_clear(work_set* ws);

#endif /* WORK_SET_H */
