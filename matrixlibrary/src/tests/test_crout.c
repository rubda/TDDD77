#include "matLib_cleanup.h"
#include <assert.h>
#include<time.h>
/*make sure typedef float value*/


int main(void) {
clock_t begin, end;
double time_spent;
begin = clock();

 matrix* a = create_matrix(4, 4);
	value temp_a[16] = { 18, 60, 57, 96,
            41, 24, 99, 58,
            14, 30, 97, 66,
            51, 13, 19, 85 };
	insert_array(temp_a, a);
	matrix* l = create_matrix(4, 4);
	matrix* u = create_matrix(4, 4);
	crout(a,l,u);

	matrix* b = create_matrix(4, 1);
	value temp_b[4] = { 18.0, 60.0, 57.0, 96.0};
	insert_array(temp_b, b);

	matrix* x = create_matrix(4, 1);

	forward_backward(l,u,x,b);

	printf("Ax=b \n");
	printf("A= \n");
	print_matrix(a);
	printf("x= \n");
	print_matrix(x);
	printf("b= \n");
	print_matrix(b);

	matrix* b_new = create_matrix(4, 1);
	multiply_matrices(a,x,b_new);
	printf("Ax= \n");
	print_matrix(b_new);
	matrix* diff = create_matrix(4, 1);
	subtract_matrices(b_new,b,diff);
printf("difference is: \n");
	print_matrix(diff);
	printf("size of value is %li \n",sizeof(value));


	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time taken was: %f \n",time_spent);
}

