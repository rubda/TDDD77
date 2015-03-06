#include<stdio.h>
#include "work_set.h"

int main() {
	work_set* ws = create_work_set(5);
	work_set_append(7);
	work_set_append(8);
	work_set_append(1);
	work_set_append(3);

	work_set_print(ws);

	work_set_remove(7);

	work_set_print(ws);


	return 0;
}