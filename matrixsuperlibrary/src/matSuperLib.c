/*
  Author: Martin Söderén
  Email: martin.soderen@gmail.com
  Date: 2015-03-31
  Description: This is the matSuperLib-library which is used to handle matrices containing matrices.
*/

/* This is the only include that is allowed in this file */
#include <matSuperLib.h>

/* a 3 x 3 matrix created with create_matrix_m(3,3);
 * 		column 	1	2	3
 * row
 * 1			[a]	[b]	[c]
 * 2			[d]	[e]	[f]
 * 3			[g]	[h]	[i]
 *
 * The rows and columns are 1-indexed
 * */

/* Creates a matrix row x col. So create_matrix(2,2) will return
 * a pointer to a matrix with 2 rows and 2 columns.
 * Returns NULL if row or col are incorrect*/
matrix_m* create_matrix_m(int row, int col) {
  if (row < 1 || col < 1) {
    return NULL;
  }

  matrix_m* mal = (matrix_m *) malloc(sizeof(matrix_m));
  mal->columns = col;
  mal->rows = row;
  mal->size = row * col;
  mal->start =  (matrix **) malloc(col * row * sizeof(matrix*));
  return mal;
}

/* Frees the matrix_m */
void free_matrix_m(matrix_m* a){
  for (int i=1;i<=a->rows;i++){
    for (int j=1;i<=a->columns;j++){
     free_matrix(get_matrix(i,j,a));
    }
    }

}

/*prints all the matrices*/
void print_matrix_m(matrix_m* mat) {
  printf("\n");
  int current_row = 1;
  int current_column = 1;
  matrix* temp = get_matrix(1, 1, mat);
  int matrix_row = temp->rows;
  int matrix_column = temp->columns;
  int rows = mat->rows * temp->rows;
  int columns = mat->columns * temp->columns;
  for (int k=0;k<rows;k++){
  printf("----------");
  }
  printf("\n");
  printf("|");
  for (int i = 1; i <= rows; i++) {
    for (int j = 1; j <= columns; j++) {
      if (current_column > matrix_column) {
        current_column = 1;
        printf("|");
      }
      temp = get_matrix(i / (matrix_row + 1) + 1, j / (matrix_column + 1) + 1,mat);
      printf(FORMAT_STRING,get_value_without_check(current_row, current_column, temp));
      current_column++;

    }
    current_row++;
    if (current_row > matrix_row) {
      current_row = 1;
      printf("|");
      printf("\n");
      for (int k=0;k<rows;k++){
      printf("----------");
      }
      printf("\n");
    }
    else{
      printf("|");
      printf("\n");
    }
  }
  printf("\n");
}

/* Inserts matrix at pos row,col in matrix mat */
bool insert_matrix(int row, int col,matrix* insert, matrix_m* mat) {
  if (row<0||col<0||row>mat->columns||col>mat->rows){
    return false;
  }
  *(mat->start+mat->columns * (row - 1)  + (col - 1))=insert;
  return true;
}

/* Returns value on location row,col in matrix mat.
 * WARNING: Only returns 0 if outside of the matrix */
matrix* get_matrix(int row, int col, matrix_m* mat) {
  return *(mat->start+mat->columns * (row - 1)  + (col - 1));
}

/* Adds a and b into c. c=a+b */
bool add_matrices_m(matrix_m* a, matrix_m* b, matrix_m* c) {
  int check = a->columns;
  if (check != b->columns || check != c->columns) {
    return false;
  }
  check = a->rows;
  if (check != b->rows || check != c->rows) {
    return false;
  }
  matrix* temp;
  for (int i=1;i<=a->rows;i++){
    for (int j=1;j<=a->columns;j++){
      temp=add_matrices_with_return(get_matrix(i,j,a),get_matrix(i,j,b));
      insert_matrix(i,j,temp ,c);
    }
  }
  return true;
}

/* Adds a and b into c. c=a+b */
matrix_m* add_matrices_m_with_return(matrix_m* a, matrix_m* b){
  matrix_m* c=create_matrix_m(a->rows,a->columns);
  if (add_matrices_m(a,b,c)){
    return c;
  }
  else{
    free_matrix_m(c);
    return NULL;
  }

}


/* Subtracts a and b into c. c=a+b */
bool subtract_matrices_m(matrix_m* a, matrix_m* b, matrix_m* c) {
  int check = a->columns;
  if (check != b->columns || check != c->columns) {
    return false;
  }
  check = a->rows;
  if (check != b->rows || check != c->rows) {
    return false;
  }
  matrix* temp;
  for (int i=1;i<=a->rows;i++){
    for (int j=1;j<=a->columns;j++){
      temp=subtract_matrices_with_return(get_matrix(i,j,a),get_matrix(i,j,b));
      insert_matrix(i,j,temp ,c);
    }
  }
  return true;
}

/* Subtracts a and b into c. c=a+b */
matrix_m* subtract_matrices_m_with_return(matrix_m* a, matrix_m* b){
  matrix_m* c=create_matrix_m(a->rows,a->columns);
  if (subtract_matrices_m(a,b,c)){
    return c;
  }
  else{
    free_matrix_m(c);
    return NULL;
  }
}

/* Multiply a and b into c. c=a*b */
bool multiply_matrices_m(matrix_m* a, matrix_m* b, matrix_m* c) {
  if ((a->columns != b->rows) || (a->rows != c->rows) || (b->columns != c->columns)) {
    return false;
  }
  size_t j = 1;
  size_t i = 1;
  size_t k = 1;
  matrix* temp_a=get_matrix(1,1,a);
  matrix* temp_b=get_matrix(1,1,b);
  matrix* sum;
  matrix* temp;

  for (; i <= a->rows; i++) {
    k = 1;
    for (; k <= b->columns; k++) {
      sum=create_matrix(temp_a->rows,temp_b->columns);
      j = 1;
      for (; j <= b->rows; j++) {
        temp=multiply_matrices_with_return(get_matrix(i,j,a),get_matrix(j,k,b));
        add_matrices(sum,temp,sum);
        free_matrix(temp);
      }
      insert_matrix(i,k,sum,c);
      print_matrix(sum);
    }
  }
  return true;
}

/* Multiply a and b by returning a pointer to c. c=a*b */
matrix_m* multiply_matrices_m_with_return(matrix_m* a, matrix_m* b){

  matrix_m* c = create_matrix_m(a->rows,b->columns);
  if (multiply_matrices_m(a,b,c)){
    return c;
  }
  else{
    free_matrix_m(c);
    return NULL;
  }
}

matrix* sum_of_all_matrices(matrix_m* a){
  matrix* temp = get_matrix(1, 1, a);
  matrix* to_return = create_matrix(temp->rows, temp->columns);
  matrix* temp1;
  for (int i = 1; i <= temp->rows; i++) {
    for (int j = 1; j <= temp->columns; j++) {
      value sum = 0;
      for (int k = 1; k <= a->rows; k++) {
        for (int l = 1; l <= a->columns; l++) {
          temp1 = get_matrix(k, l, a);
          sum += get_value(i, j, temp1);
        }
      }
      insert_value(sum, i, j, to_return);
    }
  }
  return to_return;
}



