#include "matLib.h"
#include <assert.h>
#include<time.h>
/*make sure typedef float value*/


int main(void) {
	clock_t begin, end;
	double time_spent;
	begin = clock();

	/* Creates A matrix */
	matrix* A = create_matrix(5, 2);
 	 value temp_a[10] = {
 			 			 1, -2,
            -1,  2,
            -1,  2,
             1,  0,
             0,  1,
            };
 	 insert_array(temp_a, A);

 	/* Creates Q matrix */
	matrix* Q = create_matrix(2, 2);
	value temp_q[7] = {2, 0, 2, 0};
	insert_array(temp_q, Q);

 	/* Creates d matrix */
	matrix* d = create_matrix(2, 1);
	value temp_d[7] = {-2, -5};
	insert_array(temp_d, d);

	/* Creates z matrix */
	matrix* z = create_matrix(2, 1);

	/* Creates work set */
 	work_set* w;
 	work_set_create(w, A->rows);
 	work_set_append(w, 3);
 	work_set_append(w, 5);

 	find_lagrange(Q, A, d, z, w);

	printf("A= \n");
	print_matrix(A);

	printf("Q= \n");
	print_matrix(Q);
	
	printf("d= \n");
	print_matrix(d);

	printf("z= \n");
	print_matrix(z);

	matrix* b_new = create_matrix(var, 1);
	multiply_matrices(a,x,b_new);
	printf("Ax= \n");
	print_matrix(b_new);
	matrix* diff = create_matrix(var, 1);
	subtract_matrices(b_new,b,diff);
	printf("difference is: \n");
	print_matrix(diff);
	printf("size of value is %li \n",sizeof(value));


	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time taken was: %f \n",time_spent);
}

