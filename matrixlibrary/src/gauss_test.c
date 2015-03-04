#include<matLib.h>




matrix * matrix_copy(matrix* source) {
        //TODO check
        matrix* m = create_matrix(source->rows,source->columns);
        memcpy(m->start,source->start,source->size);

     }



// A = B
matrix * gauss_eliminate(Ain,Bin) {
	float pivot = 0, val = 0;
	matrix* A = matrix_copy(Ain);

	for (int r = 1: r <= A->rows-1; r++) {

		for (int ru = r+1; ru <= A->rows; ru++) {

			pivot = get_value(ru,r,A)/get_value(r,r,A);

			for (int c = ru; c <= A->columns; c++) {
				val = get_value(ru,c,A) - get_value(r,c,A)*pivot;
				insert_value(val,ru,c,A);
			}
			insert_value(0,ru,r,A);
		}


	}
}

int main() {


	matrix* A1 = create_matrix(3,3);
	value temp_a[9] = { 1, 2, 3, 
						4, 5, 6, 
						7, 8, 9};
	insert_array(temp_a, A1);

	print_matrix(A1);

	matrix* A2;

	A2 = gauss_eliminate(A1,A1);

	print_matrix(A2);




}