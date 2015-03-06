#include "matLib.h"

//TODO apply changes to B and solve system
/* works only for symmetric, linear independant matrices (for now) */
matrix * gauss_eliminate(matrix* Ain,matrix* Bin) {
	float m = 0, val = 0, rval = 0;
	bool zero_below = true, cur_zero = false;
	

	//matrix* A = matrix_copy(Ain);
	matrix* A = Ain;
	matrix* B = Bin;


	/* reach row echelon form */
	for (int r = 1; r <= A->rows-1; r++) {
		cur_zero = false;
		zero_below = true;

		/* swap rows if necessary */
		if (get_value(r,r,A) == 0) {
			cur_zero = true;
			//find a row to swap with
			for(int ru = r+1; ru <= A->rows; ru++) {
				if (get_value(ru,r,A) != 0) {
					switch_rows(r,ru,A);
					switch_rows(r,ru,B);
					zero_below = false;
					break;
				}
			}

		}
		/* only zeroes below? */
		if (zero_below && cur_zero) {
			continue;
		}

		/* eliminate below */
		for (int ru = r+1; ru <= A->rows; ru++) {

			/* skip row if zero */
			if (get_value(ru,r,A) == 0)
			{
				continue;
			}			

			m = get_value(ru,r,A)/get_value(r,r,A);	

			//TODO 	merge theese 2 loops somehow, for better performance
			//		 	insert value can probably be done in a better way
			/* insert values in each column */
			for (int c = r; c <= A->columns; c++) {
				val = get_value(ru,c,A) - get_value(r,c,A)*m;
				insert_value(val,ru,c,A);
			}
			for (int c = 1; c <= B->columns; c++) {
				val = get_value(ru,c,B) - get_value(r,c,B)*m;
				insert_value(val,ru,c,B);
			}
			insert_value(0,ru,r,A);
			divide_row_with_scalar(get_value(r,r,A),r,A);
			divide_row_with_scalar(get_value(r,r,A),r,B);
		}
	}

	divide_row_with_scalar(get_value(A->rows,A->rows,A),A->rows,B);
	insert_value(1,A->rows,A->rows,A);
	//divide_row_with_scalar(get_value(A->rows,A->rows,A),A->rows,A);

	/* subtract backwards */
	for (int r = A->rows; r > 1; r--) {
		for (int ro = 1; ro < r; ro++) {		

			/* insert zeroes in A and subtrac row in B to the rows above */	
			m = 1/get_value(ro,r,A);
			insert_value(0,ro,r,A);
			for (int c = 1; c <= B->columns; c++) {
				val = get_value(ro,c,B) - get_value(r,c,B)*m;
				insert_value(val,ro,c,B);
			}
			//TODO add loop if A is not symmetric

		}
	}


	if (B->columns == 1) {
		return B;
	}
	//return A;
	return A;
}

int main() {


	matrix* A = create_matrix(3,3);
	matrix* B = create_matrix(3,1);

	value temp_a[9] = {	1, 2, 3,
											5, 9, 5,
											10, 50, 60};
	insert_array(temp_a, A);

	value temp_b[3] = {	6, 
											19, 
											120};
	insert_array(temp_b, B);

	print_matrix(A);

	matrix* A2;

	A2 = gauss_eliminate(A,B);

	print_matrix(A2);
	print_matrix(A);

	return 0;


}
