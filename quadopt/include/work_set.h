#ifndef WORK_SET_H
#define WORK_SET_H

/*
  Author: Alexander Yngve
  Email: aleyn573@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to work with the struct "work_set".
*/

#include <stdbool.h>

/** Structure for storing different sets */
struct work_set{
  size_t max_count; /**< Maximum number of elements in the work set */
  size_t count; /**< Number of elements in the work set. */
  size_t* data; /**< Array of elements in the work set. */
};

typedef struct work_set work_set;

/** Creates a new work set */
work_set* work_set_create(size_t ws_max);

/** Removes and deallocates the set */
bool work_set_free(work_set* ws);

/** Adds an element to the set */
bool work_set_append(work_set* ws, size_t val);

/** Removes an element from the set */
bool work_set_remove(work_set* ws, size_t val);

/** Prints all current elements in the set */
void  work_set_print(work_set* ws);

/** Checks if the set is containing the item */
bool work_set_contains(work_set* ws, size_t item);

/** Clears the set */
void work_set_clear(work_set* ws);

#endif /* WORK_SET_H */
