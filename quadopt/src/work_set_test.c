#include <stdio.h>
#include "work_set.h"

int main() {
	work_set* ws;
	work_set_create(ws, 5);
	printf("------------------");
	
	work_set_append(ws, 7);
	work_set_append(ws, 8);
	work_set_append(ws, 1);
	work_set_append(ws, 3);



	work_set_print(ws);

	work_set_remove(ws, 7);

	work_set_print(ws);


	return 0;
}