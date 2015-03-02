#include "matLib.h"
/* a 3 x 3 matrix created with create_matrix(3,3);
 * 		column 	1	2	3
 * row
 * 1			a	b	c
 * 2			d	e	f
 * 3			g	h	i
 *
 * the rows and columns are 1-indexed
 * TODO: create inversion function, a function to solva ax=b, make a test file for float and debug all functions with float,
 * make printf in print_matrix capable of printing float and int depending on value typedef, test and debug everything for double and perhaps even quad, how do we handle wrong input
 * in running program?, create a functions that copies everything from one matrix to another
 * */


/* creates a matrix row x col. So create_matrix(2,2) will return a pointer to a matrix
 * with 2 rows and 2 columns
 * TODO: check for negative col and row
 *
 * float-safe*/
matrix* create_matrix(int row, int col) {
	matrix* mal = (matrix *) malloc(sizeof(matrix));
	mal->columns = col;
	mal->rows = row;
	mal->size=row*col;
	mal->start = (value *) malloc(col * row * sizeof(value));
	return mal;
}

/*always use this when calling direct and no boundary check has been done
 * this is 1-indexed sp insert_value(1000,1,2,mat) will put 1000 in place b
 * if you use it on the matrix in the header description
 *
 * float-safe*/
bool insert_value(value insert, int row, int col, matrix* mat) {
	if (!check_boundaries(row, col, mat)) {
		return false;
	}
	*(mat->start + mat->rows * (row-1)*sizeof(value)/4 + (col-1)*sizeof(value)/4) = insert;
	return true;
}

/*only used in other functions, not directly
 * float-safe*
 * TODO fix for double*/
void insert_value_without_check(value insert, int row, int col, matrix* mat) {
	*(mat->start + mat->rows * (row-1)*sizeof(value)/4 + (col-1)*sizeof(value)/4) = insert;
}

/*always use this when calling direct and no boundary check has been done
 * this is 1-indexed get_value(1,2,mat) will return the value in place b
 * if you use it on the matrix in the header description
 *TODO do something cleaver to signal that a get failed, now it just returns 0
 *float-safe */
value get_value(int row, int col, matrix* mat) {
	if (!check_boundaries(row, col, mat)) {
		return false;
	}
	return *(mat->start + mat->rows * (row-1)*sizeof(value)/4 + (col-1)*sizeof(value)/4);
}

/*works as get_value but dont do a boundary check, only used inside functions where check are made
 * float-safe */
value get_value_without_check(int row, int col, matrix* mat) {

	return *(mat->start + mat->rows * (row-1)*sizeof(value)/4 + (col-1)*sizeof(value)/4);
}

/*checks if the position exists in the matrix
 *
 * float-safe*/
bool check_boundaries(int row, int col, matrix* mat) {
	return ((row >= 1) && (col >= 1) && (col <= mat->columns)&& (row <= mat->rows));
}

/*adds a and b into c
 *
 * float-safe*/
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

	size = a->columns * a->rows*sizeof(value)/4;

	for (i = 0; i < size; i+=sizeof(value)/4) {
		*(c->start + i) = *(a->start + i) + *(b->start + i);
	}
	return true;

}

/*subtract a and b into c
 *
 * float-safe*/
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
	size = a->columns * a->rows*sizeof(value)/4;

	for (i = 0; i < size; i+=sizeof(value)/4) {
		*(c->start + i) = *(a->start + i) - *(b->start + i);
	}
	return true;

}

/*return the sum of a row in matrix mat
 *
 * float safe*/
value sum_of_row(int row, matrix* mat) {
	if (!check_boundaries(row,1,mat)){
		return false;
	}
	size_t i = 0;
	value *start = mat->start + (row - 1) * mat->columns*sizeof(value)/4;
	value to_return = 0;
	for (; i < mat->columns; i+=sizeof(value)/4) {
		to_return += *(start + i);
	}
	return to_return;
}

/*return the sum of a column in matrix mat
 *
 * float-safe*/
value sum_of_column(int column, matrix* mat) {
	if (!check_boundaries(1,column,mat)){
		return false;
	}
	size_t i = 0;
	value *start = mat->start + (column - 1)*sizeof(value)/4;
	value to_return = 0;
	for (; i < mat->rows; i++) {
		to_return += *(start + i * mat->columns*sizeof(value)/4);
	}
	return to_return;
}

/*takes row vector from matrix a and puts it into b which also is a row vector
 *however get_sub_matrix should be faster
 *TODO this should be done with memcpy, should be alot faster
 * float-safe*/
 bool get_row_vector(int row,matrix* a,matrix* b){
		if (!check_boundaries(row,1,a)){
			return false;
		}
	 if (b->rows!=1 || b->columns!=a->columns){
		 return false;
	 }
		size_t i = 0;
		value *start = a->start + (row - 1) * a->columns*sizeof(value)/4;
		for (; i < a->columns; i++) {
			*(b->start+i*sizeof(value)/4)= *(start + i*sizeof(value)/4);
		}
		return true;
 }
 /*Inserts row vector a into b:s row*/
 bool insert_row_vector(int row,matrix* a,matrix* b){
	 if((a->columns!=b->columns)||(a->rows!=1)){
		 return false;
	 }
	 value *start = b->start + (row - 1) * b->columns*sizeof(value)/4;
	 size_t number_of_bytes=b->columns*sizeof(value);
	 /*memcpy(dest,src,number_of_bytes)*/
	 memcpy((void *)start,(void *)(a->start),number_of_bytes);

 }

 /*takes column vector from matrix a and puts it into b which also is a column vector
  *however get_sub_matrix should be faster
  *
  * float-safe*/
 bool get_column_vector(int column, matrix* a,matrix* b) {
		if (!check_boundaries(1,column,a)){
			return false;
		}
	 if (b->columns!=1 || b->rows!=a->rows){
		 return false;
	 }

 	size_t i = 0;
 	value *start = a->start + (column - 1)*sizeof(value)/4;
 	for (; i < a->rows; i++) {
 		*(b->start+i*sizeof(value)/4)= *(start + i * a->columns*sizeof(value)/4);
 	}
 	return true;
 }
/*solve Ax=b system the x */
 bool solver(matrix* a,matrix* b,matrix* c){
	 if ((a->columns!=c->rows)||(a->rows!=b->rows)||(c->columns!=b->columns)){
		 return false;

	 }

 }


/*multiply a and b into c
 *
 * float-safe*/
bool multiply_matrices(matrix* a, matrix* b,matrix* c) {
	if ((a->columns != b->rows) || (a->rows != c->rows)
			|| (b->columns != c->columns)) {
		return false;
	}
	size_t j = 1;
	size_t i = 1;
	size_t k = 1;
	value sum = 0;
	value temp_a=0;
	value temp_b=0;
	for (; i <= a->rows; i++) {
		k=1;
		for (; k <= b->columns; k++) {
			sum = 0;
			j=1;
			for (; j <= b->rows; j++) {
				temp_a=get_value_without_check(i, j, a);
				temp_b=get_value_without_check(j, k, b);
				//sum += get_value_without_check(i, j, a) * get_value_without_check(j, k, b);
				sum+=temp_a*temp_b;
			}
			insert_value_without_check(sum, i, k, c);
		}
	}
	return true;
}

/*transposes matrix a into b
 *
 * float-safe*/
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
	return true;
}

/*calculates the inverse of a and puts it into b, this should be used with the value typedef set to float
 * RETURN WILL PROBABLY BE WRONG IF USED IN INT MODE
 *
 * float-safe*/
bool inverse_of_2x2_matrix(matrix* a,matrix* b){
	if ((a->columns!=2)||(a->rows!=2)||(b->rows!=2)||(b->columns!=2)){
		return false;
	}
	size_t word_size=sizeof(value)/4;
	value determinant=1 / ( (*(a->start)) * (*(a->start+3*word_size)) - (*(a->start+1*word_size))*(*(a->start+2*word_size)) );
	*(b->start)=determinant*(*(a->start+3*word_size));
	*(b->start+1)=-determinant*(*(a->start+1*word_size));
	*(b->start+2)=-determinant*(*(a->start+2*word_size));
	*(b->start+4)=determinant*(*(a->start));
	return true;
}

/*multiplies matrix mat with scal
 *
 * float-safe*/
void multiply_matrix_with_scalar(int scal,matrix* mat){
	size_t size=mat->size;
	size_t i=0;
	size_t word_size=sizeof(value)/4;
	for (;i<size;i++){
		*(mat->start+i*word_size)*=scal;
	}
}

/*takes the submatrix defined by start_row,end_row,start_col,end_col and put it into matrix b
 * float-safe*/
bool get_sub_matrix(int start_row,int end_row,int start_col,int end_col,matrix* a,matrix* b){
  if (!check_boundaries(start_row,start_col,a)||
      !check_boundaries(end_row,end_col,a)){
    return false;
  }
  if ((b->rows!=(end_row-start_row+1))||
      (b->columns!=(end_col-start_col+1))){
    return false;
  }
  /*how many rows to copy*/
  size_t size=end_row-start_row+1;

  /*how many bytes to skip ahead in a matrix */
  size_t start=((start_row-1)*a->columns+start_col-1)*sizeof(value)/4;

  /*how many bytes to skip in each step */
  size_t step = (a->columns)*sizeof(value)/4;

  /*how many words a row to copy is*/
  size_t row_length=(end_row-start_row+1)*sizeof(value)/4;

  /*how many bytes to copy a row is*/
  size_t number_of_bytes=(end_row-start_row+1)*sizeof(value);

  size_t i=0;
  for(;i<size;i++){
    memcpy((void *)(b->start+row_length*i),(void *)(a->start+start+step*i),number_of_bytes);
  }
  return true;
}

/*insert a array into the matrix, the array must have the same size as number of total elements in the matrix*
 * TODO check elements in array*/
bool insert_array(value arr[], matrix* mat) {
	size_t size = mat->size;
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

/*returns true if matrices a and b look the same*/
bool compare_matrices(matrix* a,matrix* b){
if ((a->columns!=b->columns)||(a->rows!=b->rows)){
	return false;
}
size_t number_of_bytes =a->size*sizeof(value);
return 0==memcmp((void *)(b->start),(void *)(a->start),number_of_bytes);
}

/*return true if the matrix are the same*/
bool is_matrix(matrix* a,matrix* b){
	return ((a->size==b->size)&&(a->rows==b->rows)&&(a->columns==b->columns)&&(a->start==b->start));
}

/*prints the matrix*/
void print_matrix(matrix* mat) {
	size_t size = mat->columns * mat->rows;
	int col = 0;
	size_t i = 0;
	printf("\n");
	for (; i < size; i++) {
		printf("%i " , *(mat->start + i*sizeof(value)/4));
		col++;
		if (col >= mat->columns) {
			col = 0;
			printf("\n");
		}
	}
	printf("\n");
}

int raise(int base, int exp) {
	int i = 0;
	int temp = base;
	for (; i < exp; i++) {
		temp *= base;
	}
	return temp;
}

/*return a row matrix containing all the primes factors of n*/
matrix* prime_factorization(int number) {
	if (number <= 0) {
		matrix* to_return = create_matrix(1, 1);
		insert_value(0, 1, 1, to_return);
		return to_return;
	}
	int div = 2;
	int memory = 1;
	int * primes = (int*) malloc(memory * sizeof(int));
	int counter = 0;

	while (number != 0) {
		if (number % div != 0)
			div = div + 1;
		else {
			number = number / div;
			if (counter == memory) {
				primes = (int*) realloc((void *) primes,
						2 * memory * sizeof(int));
				memory *= 2;
			}
			*(primes + counter) = div;
			counter++;
			if (number == 1)
				break;
		}
	}
	matrix* to_return = create_matrix(1, counter);
	int i;
	for (i = 0; i < counter; i++) {
		insert_value(*(primes + i), 1, i + 1, to_return);
	}
	free(primes);
	return to_return;

void free_matrix(matrix* mat){
  free(mat->start);
  free(mat);
}
