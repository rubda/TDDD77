#ifndef WORK_SET_H
#define WORK_SET_H

/* just a simple structure for storing different sets */
struct work_set {
	int count;
	int* data;
};

typedef struct work_set work_set;


/* creates a new work set */
bool work_set_create(work_set* ws, int ws_max);

/* adds an element to the set */
bool work_set_append(,work_set* ws, int val);

/* removes an element from the set */
bool work_set_remove(work_set* ws, int val);

/* removes and deallocates the set */
bool free_work_set(work_set* ws);

#endif /* WORK_SET_H */