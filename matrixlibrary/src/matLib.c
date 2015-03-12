/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-05
  Description: This is the matLib-library which is used to handle matrices.
*/

/* This is the only include that is allowed in this file */
#include <matLib.h>

/* a 3 x 3 matrix created with create_matrix(3,3);
 * 		column 	1	2	3
 * row
 * 1			a	b	c
 * 2			d	e	f
 * 3			g	h	i
 *
 * The rows and columns are 1-indexed
 * */

/* Creates a matrix row x col. So create_matrix(2,2) will return
 * a pointer to a matrix with 2 rows and 2 columns.
 * Returns NULL if row or col are incorrect*/
matrix* create_matrix(int row, int col) {
	if (row < 1 || col < 1) {
		return NULL;
	}
	matrix* mal = (matrix *) malloc(sizeof(matrix));
	mal->columns = col;
	mal->rows = row;
	mal->size = row * col;
	mal->start = (value *) malloc(col * row * sizeof(value));
	return mal;
}

/* free the memory allocated by matrix mat */
void free_matrix(matrix* mat) {
	free(mat->start);
	free(mat);
}

/* calculate the vector product */
value vector_product(matrix* r, matrix* v) {
  //TODO check
  value ans = 0;
  for (int i = 1; i <= r->rows; i++) {
    //TODO access the memory directly might be faster
    ans += get_value_without_check(i,1,r) * get_value_without_check(i,1,v);
  }
  return ans;
}

/*prints the matrix*/
void print_matrix(matrix* mat) {
	int col = 1;
	int row = 1;
	size_t i = 1;
	for (; i <= mat->size; i++) {
		printf(FORMAT_STRING, get_value_without_check(row, col, mat));
		col++;
		if (col > mat->columns) {
			col = 1;
			row++;
			printf("\n");
		}
	}
	printf("\n");
}

/* Checks if the position exists in the matrix */
bool check_boundaries(int row, int col, matrix* mat) {
	return ((row >= 1) && (col >= 1) && (col <= mat->columns)
			&& (row <= mat->rows));
}

/* Insert a array into the matrix, the array must have the same size as number of total elements in the matrix */
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
bool compare_matrices(matrix* a, matrix* b) {
	if ((a->columns != b->columns) || (a->rows != b->rows)) {
		return false;
	}
	size_t number_of_bytes = a->size * sizeof(value);
	return 0 == memcmp((void *) (b->start), (void *) (a->start), number_of_bytes);
}

/* Return true if the matrix are the same */
bool is_matrix(matrix* a, matrix* b) {
	return ((a->size == b->size) && (a->rows == b->rows)
			&& (a->columns == b->columns) && (a->start == b->start));
}


/* Inserts value at pos rowm,col in matrix mat */
bool insert_value(value insert,int row, int col, matrix* mat) {
	if (!check_boundaries(row, col, mat)) {
		return false;
	}
	value* start = mat->start + mat->columns * (row - 1)  + (col - 1);
	*(start) = insert;
	return true;
}

/* As insert_value without check */
void insert_value_without_check(value insert, int row, int col, matrix* mat) {
	*(mat->start + mat->columns * (row - 1) + (col - 1)) = insert;
}

/* Returns value on location row,col in matrix mat.
 * WARNING: Only returns 0 if outside of the matrix */
value get_value(int row, int col, matrix* mat) {
	if (!check_boundaries(row, col, mat)) {
		return false;
	}
	return *(mat->start + mat->columns * (row - 1) + (col - 1));
}

/* As get_value without check */
value get_value_without_check(int row, int col, matrix* mat) {
	return *(mat->start + mat->columns * (row - 1) + (col - 1));
}

/* Adds a and b into c. c=a+b */
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
	size = a->size;
	for (i = 0; i < size; i++) {
		*(c->start + i) = *(a->start + i) + *(b->start + i);
	}
	return true;
}

/* Subtract a and b into c. c=a-b */
bool subtract_matrices(matrix* a, matrix* b, matrix* c) {
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
	size = a->size;
	for (i = 0; i < size; i++) {
		*(c->start + i) = *(a->start + i) - *(b->start + i);
	}
	return true;
}

/* Multiply a and b into c. c=a*b */
bool multiply_matrices(matrix* a, matrix* b, matrix* c) {
	if ((a->columns != b->rows) || (a->rows != c->rows)
			|| (b->columns != c->columns)) {
		return false;
	}
	size_t j = 1;
	size_t i = 1;
	size_t k = 1;
	value sum = 0;
	for (; i <= a->rows; i++) {
		k = 1;
		for (; k <= b->columns; k++) {
			sum = 0;
			j = 1;
			for (; j <= b->rows; j++) {
				sum += get_value_without_check(i, j, a)
						* get_value_without_check(j, k, b);
			}

			insert_value_without_check(sum, i, k, c);
		}
	}
	return true;
}

/* Solves Ax=B */
bool solve_linear(matrix* a,matrix* x, matrix* b){
	matrix* u=create_matrix(a->rows,a->columns);
	matrix* l=create_matrix(a->rows,a->columns);
	if (!crout(a,l,u)){
		return false;
	}
	forward_backward(l,u,x,b);
	free_matrix(u);
	free_matrix(l);
	return true;
}

/* Crout algorithm to divide matrix a into l and u that holds a=lu */
bool crout(matrix* a, matrix* l, matrix* u) {
	if (a->rows != a->columns) {
		return;
	}
	int check = a->rows;
	if (check != l->rows || check != u->rows) {
		return;
	}
	check = a->columns;
	if (check != l->columns || check != u->columns) {
		return;
	}
	int i, j, k;
	double sum = 0;
	int n = a->columns;
	for (i = 1; i <= n; i++) {
		insert_value(1, i, i, u);
	}
	for (j = 1; j <= n; j++) {
		for (i = j; i <= n; i++) {
			sum = 0;
			for (k = 1; k < j; k++) {
				sum = sum + get_value(i, k, l) * get_value(k, j, u);
			}
			insert_value(get_value(i, j, a) - sum, i, j, l);
		}
		for (i = j; i <= n; i++) {
			sum = 0;
			for (k = 1; k < j; k++) {
				sum = sum + get_value(j, k, l) * get_value(k, i, u);
			}

			if (get_value(j, j, l) == 0) {
				return false;
			}
			insert_value((get_value(j, i, a) - sum) / get_value(j, j, l), j, i,
					u);
		}
	}
	return true;
}

/* Solves lux=b using backward and forward substitution */
void forward_backward(matrix* l, matrix* u, matrix* x, matrix* b) {
	if (l->rows != u->rows || l->columns != u->columns
			|| l->rows != l->columns) {
		return;
	}
	if (x->rows != b->rows || x->columns != b->columns || x->columns != 1) {
		return;
	}
	if (l->columns != x->rows) {
		return;
	}

	int i = 1;
	int j = 1;
	int n = l->rows;
	value temp;
	matrix* y = create_matrix(x->rows, x->columns);
	for (i = 1; i <= n; i++) {
		insert_value(get_value(i, 1, b), i, 1, y);
		for (j = 1; j < i; j++) {
			temp = get_value(i, 1, y) - get_value(i, j, l) * get_value(j, 1, y);
			insert_value(temp, i, 1, y);
		}
		temp = get_value(i, 1, y) / get_value(i, i, l);
		insert_value(temp, i, 1, y);
	}
	for (i = n; i >= 1; i--) {
		insert_value(get_value(i, 1, y), i, 1, x);
		for (j = i + 1; j <= n; j++) {
			temp = get_value(i, 1, x) - get_value(i, j, u) * get_value(j, 1, x);
			insert_value(temp, i, 1, x);
		}
		temp = get_value(i, 1, x) / get_value(i, i, u);
		insert_value(temp, i, 1, x);
	}
	free_matrix(y);
}

/* Adds each element in row1 and row 2 and puts the result on row2 */
void add_rows(int row1, int row2, matrix* a) {
	value* start1 = a->start + a->columns * (row1 - 1);
	value* start2 = a->start + a->columns * (row2 - 1);
	size_t i = 0;
	for (; i < a->columns; i++) {
		*(start2 + i) += *(start1 + i);
	}
}

/* Transposes matrix a into b */
bool transpose_matrix(matrix* a, matrix*b) {
	if ((a->columns != b->rows) || (a->rows != b->columns)) {
		return false;
	}
	size_t size = a->size;
	int row = 1;
	int col = 1;
	size_t i = 0;
	for (; i < size; i++) {
		insert_value_without_check(*(a->start + i), row, col, b);
		row++;
		if (row > b->rows) {
			row = 1;
			col++;
		}
	}
	return true;
}

/* Return the sum of a row in matrix mat */
value sum_of_row(int row, matrix* mat) {
	if (!check_boundaries(row, 1, mat)) {
		return false;
	}
	size_t i = 0;
	value *start = mat->start + (row - 1) * mat->columns;
	value to_return = 0;
	for (; i < mat->columns; i++) {
		to_return += *(start + i);
	}
	return to_return;
}

/* Return the sum of a column in matrix mat */
value sum_of_column(int column, matrix* mat) {
	if (!check_boundaries(1, column, mat)) {
		return false;
	}
	size_t i = 0;
	value *start = mat->start + (column - 1);
	value to_return = 0;
	for (; i < mat->rows; i++) {
		to_return += *(start + i * mat->columns);
	}
	return to_return;
}

/* Multiplies matrix mat with scalar */
void multiply_matrix_with_scalar(value scal, matrix* mat) {
	size_t size = mat->size;
	size_t i = 0;
	for (; i < size; i++) {
		*(mat->start + i ) *= scal;
	}
}

/* Divides matrix mat with scalar */
void divide_matrix_with_scalar(value scal, matrix* mat) {
	size_t size = mat->size;
	size_t i = 0;
	for (; i < size; i++) {
		*(mat->start + i ) /= scal;
	}
}

/* Multiplies a row with a scalar */
void multiply_row_with_scalar(value scal, int row, matrix* mat) {
	value* start = mat->start + (row - 1) * mat->columns;
	size_t i = 0;
	for (; i < mat->columns; i++) {
		*(start + i) *= scal;
	}
}

/* Divides a row with a scalar */
void divide_row_with_scalar(value scal, int row, matrix* mat) {
	value* start = mat->start + (row - 1) * mat->columns;
	size_t i = 0;
	for (; i < mat->columns; i++) {
		*(start + i) /= scal;
	}
}

/* Multiplies a column with a scalar */
void multiply_column_with_scalar(value scal, int col, matrix* mat) {
	value* start = mat->start + (col - 1);
	size_t i = 0;
	size_t step = mat->rows ;
	for (; i < mat->rows; i++) {
		*(start + i * step) *= scal;
	}
}

/* Divides a column with a scalar */
void divide_column_with_scalar(value scal, int col, matrix* mat) {
	value* start = mat->start + (col - 1);
	size_t i = 0;
	size_t step = mat->rows ;
	for (; i < mat->rows; i++) {
		*(start + i * step) /= scal;
	}
}

/* Takes row vector from matrix a and puts it into b which also is a row vector
 *however get_sub_matrix should be faster */
bool get_row_vector(int row, matrix* a, matrix* b) {
	if (!check_boundaries(row, 1, a)) {
		return false;
	}
	if (b->rows != 1 || b->columns != a->columns) {
		return false;
	}
	size_t i = 0;
	value *start = a->start + (row - 1) * a->columns;
	for (; i < a->columns; i++) {
		*(b->start + i ) = *(start + i);
	}
	return true;
}

/* Inserts row vector a into b:s row */
bool insert_row_vector(int row, matrix* a, matrix* b) {
	if ((a->columns != b->columns) || (a->rows != 1)) {
		return false;
	}
	value *start = b->start + (row - 1) * b->columns;
	size_t number_of_bytes = b->columns * sizeof(value);
	memcpy((void *) start, (void *) (a->start), number_of_bytes);
	return true;
}

/* Switches rows in a */
bool switch_rows(int row1, int row2, matrix* a) {
	matrix* b = create_matrix(1, a->columns);
	if (!get_row_vector(row1, a, b)) {
		return false;
	}
	matrix* c = create_matrix(1, a->columns);
	if (!get_row_vector(row2, a, c)) {
		return false;
	}
	insert_row_vector(row1, c, a);
	insert_row_vector(row2, b, a);
	free_matrix(c);
	free_matrix(b);
	return true;
}

/* Takes column vector from matrix a and puts it into b which also is a column vector
 *however get_sub_matrix should be faster */
bool get_column_vector(int column, matrix* a, matrix* b) {
	if (!check_boundaries(1, column, a)) {
		return false;
	}
	if (b->columns != 1 || b->rows != a->rows) {
		return false;
	}

	size_t i = 0;
	value *start = a->start + (column - 1);
	for (; i < a->rows; i++) {
		*(b->start + i) = *(start + i * a->columns);
	}
	return true;
}

/* Inserts column vector a into matrix b at position column */
bool insert_column_vector(int column, matrix *a, matrix* b) {
	if (a->columns != 1 || b->rows != a->rows) {
		return false;
	}
	value *start = b->start + (column - 1);
	size_t size = b->columns;
	size_t i = 0;
	for (; i < size; i++) {
		memcpy((start + i * size), a->start + i, sizeof(value));
	}
	return true;
}

/* Takes the submatrix defined by start_row,end_row,start_col,end_col and put it into matrix b */
bool get_sub_matrix(int start_row, int end_row, int start_col, int end_col, matrix* a, matrix* b) {
  if (!check_boundaries(start_row, start_col, a)
      || !check_boundaries(end_row, end_col, a)) {
    return false;
  }
  if ((b->rows != (end_row - start_row + 1))
      || (b->columns != (end_col - start_col + 1))) {
    return false;
  }

  start_row -= 1;
  end_row -= 1;
  start_col -= 1;
  end_col -= 1;

  size_t a_row_size = a->columns;
  size_t b_row_size = b->columns;
  size_t offset = a_row_size * start_row + start_col;
  size_t num_rows = end_row - start_row + 1;
  size_t bytes_per_row = (end_col - start_col + 1) * sizeof(value);

  for(int i = 0; i < num_rows; i++){
    void* to = b->start + b_row_size * i;
    void* from = a->start + offset + a_row_size * i;
    memcpy(to, from, bytes_per_row);
  }

  return true;
}

/* Copy and return new matrix. */
matrix* matrix_copy(matrix* source) {
  //TODO check
  matrix* m = create_matrix(source->rows,source->columns);
  memcpy(m->start,source->start,source->size * sizeof(value));
  return m;
}

/* Cpoies all the data from matrix A into matrix B */
void matrix_copy_data(matrix* A, matrix* B) {
	//TODO check
	for (int i = 1; i <= A->rows; i++) {
		for (int j = 1; j <= A->columns; j++) {
			insert_value_without_check(get_value_without_check(i,j,A),i,j,B);
		}
	}
}

/* checks if all elements in a matrix is equal to zero */
bool is_zero_matrix(matrix* v) {
  //TODO check
  for (int i = 1; i <= v->rows; i++) {
    for(int j = 1; j <= v->columns; j++){
      if (get_value_without_check(i,j,v) != 0) {
	return false;
      }
    }
  }
  return true;
}

/* checks if all elements in a matrix is positive */
bool is_non_negative_matrix(matrix* v) {
  //TODO check
  for (int i = 1; i <= v->rows; i++) {
    for(int j = 1; j <= v->columns; j++){
      if (get_value_without_check(i,j,v) < 0) {
	return false;
      }
    }
  }
  return true;
}

/* checks if all elements along the diagonal in a symmetric matrix is positiv */
bool is_non_negative_diagonal_matrix(matrix* A) {
  //TODO check
  for (int i = 1; i <= A->rows; i++) {
    if (get_value_without_check(i,i,A) < 0) {
      return false;
    }
  }
  return true;
}
