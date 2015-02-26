#include "matLib.h"
#include <time.h>
/* a 3 x 3 matrix created with create_matrix(3,3);
 * 		column 	1	2	3
 * row
 * 1			a	b	c
 * 2			d	e	f
 * 3			g	h	i
 *
 * the rows and columns are 1-indexed
 * */


/* creates a matrix row x col. So create_matrix(2,2) will return a pointer to a matrix
 * with 2 rows and 2 columns
 * TODO: check for negative col and row
 * */
matrix* create_matrix(int row, int col) {
	matrix* mal = (matrix *) malloc(sizeof(matrix *));
	mal->columns = col;
	mal->rows = row;
	mal->size=row*col;
	mal->start = (value *) malloc(col * row * sizeof(value));
	return mal;
}

/*always use this when calling direct and no boundary check has been done
 * this is 1-indexed sp insert_value(1000,1,2,mat) will put 1000 in place b
 * if you use it on the matrix in the header description
 * */
bool insert_value(value insert, int row, int col, matrix* mat) {
	if (!check_boundaries(row, col, mat)) {
		return false;
	}
	*(mat->start + mat->rows * (row-1) + (col-1)) = insert;
	return true;
}

/*only used in other functions, not directly*/
void insert_value_without_check(value insert, int row, int col, matrix* mat) {
	*(mat->start + mat->rows * (row-1) + (col-1)) = insert;
}

/*always use this when calling direct and no boundary check has been done
 * this is 1-indexed get_value(1,2,mat) will return the value in place b
 * if you use it on the matrix in the header description
 * */
value get_value(int row, int col, matrix* mat) {
	if (!check_boundaries(row, col, mat)) {
		return false;
	}
	return *(mat->start + mat->rows * (row-1) + (col-1));
}

/*works as get_value but dont do a boundary check, only used inside functions where check are made  */
value get_value_without_check(int row, int col, matrix* mat) {

	return *(mat->start + mat->rows * (row-1) + (col-1));
}

/*checks if the position exists in the matrix*/
bool check_boundaries(int row, int col, matrix* mat) {
	return ((row >= 1) && (col >= 1) && (col <= mat->columns)&& (row <= mat->rows));
}

/*adds a and b into c*/
bool add_matrices(matrix* a, matrix* b, matrix* c) {
	size_t size;
	size_t i;
	int check = a->columns;
	if (check != b->columns || check != c->columns) {
		return false;
	}
	check = a->rows;
	if (check != b->rows || check != c->rows) {
		return false;
	}
	size = a->columns * a->rows;

	for (i = 0; i < size; i++) {
		*(c->start + i) = *(a->start + i) + *(b->start + i);
	}
	return true;

}

/*subtract a and b into c*/
bool subtract_matrices(matrix* a, matrix* b,matrix* c) {
	size_t size;
	size_t i;
	int check = a->columns;
	if (check != b->columns || check != c->columns) {
		return false;
	}
	check = a->rows;
	if (check != b->rows || check != c->rows) {
		return false;
	}
	size = a->columns * a->rows;

	for (i = 0; i < size; i++) {
		*(c->start + i) = *(a->start + i) - *(b->start + i);
	}
	return true;

}

/*return the sum of a row in matrix mat*/
value sum_of_row(int row, matrix* mat) {
	size_t i = 0;
	value *start = mat->start + (row - 1) * mat->columns;
	value to_return = 0;
	for (; i < mat->columns; i++) {
		to_return += *(start + i);
	}
	return to_return;
}

/*return the sum of a column in matrix mat*/
value sum_of_column(int column, matrix* mat) {
	size_t i = 0;
	value *start = mat->start + column - 1;
	value to_return = 0;
	for (; i < mat->rows; i++) {
		to_return += *(start + i * mat->columns);
	}
	return to_return;
}
/*takes row vector from matrix a and puts it into b which also is a row vector*/
 bool get_row_vector(int row,matrix* a,matrix* b){
	 if (b->rows!=1 || b->columns!=a->columns){
		 return false;
	 }
		size_t i = 0;
		value *start = a->start + (row - 1) * a->columns;
		for (; i < a->columns; i++) {
			*(b->start+i)= *(start + i);
		}
		return true;
 }

 /*takes column vector from matrix a and puts it into b which also is a column vector*/
 bool get_column_vector(int column, matrix* a,matrix* b) {
	 if (b->columns!=1 || b->rows!=a->rows){
		 return false;
	 }
 	size_t i = 0;
 	value *start = a->start + column - 1;
 	for (; i < a->rows; i++) {
 		*(b->start+i)= *(start + i * a->columns);
 	}
 	return true;
 }


/*multiply a and b into c*/
bool multiply_matrices(matrix* a, matrix* b,matrix* c) {
	if ((a->columns != b->rows) || (a->rows != c->rows)
			|| (b->columns != c->columns)) {
		return false;
	}
	size_t j = 1;
	size_t i = 1;
	size_t k = 1;
	value sum = 0;
	for (; i <= a->rows; i++) {
		k=1;
		for (; k <= b->columns; k++) {
			sum = 0;
			j=1;
			for (; j <= b->rows; j++) {
				sum += get_value_without_check(i, j, a)
						* get_value_without_check(j, k, b);
			}
			insert_value_without_check(sum, i, k, c);

		}
	}

	return true;

}

/*transposes matrix a into b*/
bool transpose_matrix(matrix* a, matrix*b){
	if((a->columns!=b->rows)||(a->rows!=b->columns)){
		return false;
	}
	size_t size=a->size;
	int row = 1;
	int col = 1;
	size_t i = 0;
	for (; i < size; i++) {
		insert_value_without_check(*(a->start+i), row, col, b);
		row++;
		if (row>b->rows){
		row=1;
		col++;

		}
	}
}

/*calculates the inverse of a and puts it into b, this should be used with the value typedef set to float
 * RETURN WILL PROBABLY BE WRONG IF USED IN INT MODE*/
bool inverse_of_2x2_matrix(matrix* a,matrix* b){
	if ((a->columns!=2)||(a->rows!=2)||(b->rows!=2)||(b->columns!=2)){
		return false;
	}
	value determinant=1 / ( (*(a->start)) * (*(a->start+3)) - (*(a->start+1))*(*(a->start+2)) );
	*(b->start)=determinant*(*(a->start+3));
	*(b->start+1)=-determinant*(*(a->start+1));
	*(b->start+2)=-determinant*(*(a->start+2));
	*(b->start+4)=determinant*(*(a->start));
	return true;
}

/*multiplies matrix mat with scal*/
void multiply_matrix_with_scalar(int scal,matrix* mat){
	size_t size=mat->size;
	size_t i=0;
	for (;i<size;i++){
		*(mat->start+i)+=scal;
	}
}
/*takes the submatrix defined by start_row,end_row,start_col,end_col and put it into matrix b*/
bool get_sub_matrix(int start_row,int end_row,int start_col,int end_col,matrix* a,matrix* b){
if (!check_boundaries(start_row,start_col,a)||!check_boundaries(end_row,end_col,a)){
return false;
}
if ((b->rows!=(end_row-start_row+1))||(b->columns!=(end_col-start_col+1))){
	return false;
}


}


/*insert a array into the matrix, the array must have the same size as number of total elements in the matrix*
 * TODO check elements in array*/
bool insert_array(value arr[], matrix* mat) {
	size_t size = mat->columns * mat->rows;
	int row = 1;
	int col = 1;
	size_t i = 0;
	for (; i < size; i++) {
		insert_value_without_check(arr[i], row, col, mat);
		col++;
		if (col > mat->columns) {
			col = 1;
			row++;
		}
	}
	return true;
}

/*prints the matrix*/
void print_matrix(matrix* mat) {
	size_t size = mat->columns * mat->rows;
	int col = 0;
	size_t i = 0;
	printf("\n");
	for (; i < size; i++) {
		printf("%i " , *(mat->start + i));
		col++;
		if (col >= mat->columns) {
			col = 0;
			printf("\n");
		}
	}
	printf("\n");
}

/*test program*/
int main(void) {
	matrix* b;
	matrix* c;
	matrix* d;
	clock_t begin, end;
	double time_spent;
	begin = clock();
	matrix* a = create_matrix(4, 4);
	value temp_a[16] = { 18, 60, 57, 96,
            41, 24, 99, 58,
            14, 30, 97, 66,
            51, 13, 19, 85 };
	insert_array(temp_a, a);
	print_matrix(a);

	b = create_matrix(4, 4);
	value temp_b[16] = { 18, 60, 57, 96,
            41, 24, 99, 58,
            14, 30, 97, 66,
            51, 13, 19, 85 };
	insert_array(temp_b, b);
	print_matrix(b);

	c = create_matrix(4, 4);

	multiply_matrices(a, b, c);
	print_matrix(c);
	d = create_matrix(4, 4);
	transpose_matrix(c,d);
	print_matrix(d);
	return 0;
}
