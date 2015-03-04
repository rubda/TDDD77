#include "matLib.h"




matrix * matrix_copy(matrix* source) {
        //TODO check
        matrix* m = create_matrix(source->rows,source->columns);
        memcpy(m->start,source->start,source->size);
        return m;
     }



//TODO apply changes to B and solve system
/* works only for symmetric, linear independant matrices (for now) */
matrix * gauss_eliminate(matrix* Ain,matrix* Bin) {
	float m = 0, val = 0, rval = 0;
	bool zero_below = true, cur_zero = false;
	

	//matrix* A = matrix_copy(Ain);
	matrix* A = Ain;


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

			for (int c = r; c <= A->columns; c++) {
				val = get_value(ru,c,A) - get_value(r,c,A)*m;
				insert_value(val,ru,c,A);
			}
			insert_value(0,ru,r,A);
		}
	}

	/* subtitute backwards */
	for (int r = A->rows-1; r >= 1; r--) {
		for (int c = r+1; c <= A->columns; c++) {
			//TODO alot
			insert_value(0,r,c,A);

		}
	}

	return A;
}

int main() {


	matrix* A1 = create_matrix(4,4);
	value temp_a[16] = {1, 2, 3, 4,
						5, 9, 5, 2,
						10, 50, 60, 660,
						10,2,1,1};
	insert_array(temp_a, A1);

	print_matrix(A1);

	matrix* A2;

	A2 = gauss_eliminate(A1,A1);

	print_matrix(A2);

	return 0;


}