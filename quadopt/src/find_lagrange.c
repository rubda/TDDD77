#include <matLib.h>
#include <work_set.h>
#include <stdbool.h>

/* Finds the lagrange multipliers and removes constraints */
bool find_lagrange(matrix* Q, matrix* A, matrix* d, matrix* z, work_set* w){
	
	/* Calculates g = Gz + d */
	matrix* g = create_matrix(Q->rows, 1); 
	multiply_matrices(Q, z, g);
	add_matrices(g, d, g);

	/* Puts all the related conditions to w_mat depending on the work_set */
	matrix* tmp_mat = create_matrix(A->rows, w->count);
	matrix* w_mat = create_matrix(A->rows, w->count); 
	for (i = 1; i <= w->count; i++){
		get_row_vector(w->data[i], A, tmp_mat);
	}
	transpose_matrix(tmp_mat, w_mat);

	/* Solves the system W_mat * x = g */
	matrix* solved = create_matrix(A->rows, w->count);
	solve_linear(w_mat, solved, g);

	/* Finds the lowest lagrange value */
	int min_row = 1;
	value min_value = get_value(1, 1, solved);
	value temp;

	for (i = 2; i <= w->count; i++){
		temp = get_value(i, 1, solved);
		if (temp < min_value){
			min_value = temp;
			min_row = i;
		}
	}

	/* Removes the lowest lagrange value from the work set */
	work_set_remove(w, w->data[min_row]);

	/* Success */
	return true;
}