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
matrix* create_matrix(size_t row, size_t col) {
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

/* Is normally not needed for this implementation but might be needed on others */
matrix* create_zero_matrix(size_t row,size_t col){
  if (row < 1 || col < 1) {
    return NULL;
  }

  matrix* mal = (matrix *) malloc(sizeof(matrix));
  mal->columns = col;
  mal->rows = row;
  mal->size = row * col;
  mal->start = (value *) calloc(1,col * row * sizeof(value));
  return mal;
}

/* Creates a identity matrix */
matrix* create_identity_matrix(size_t row,size_t col){
  if (row!=col){
    return NULL;
  }
  matrix* to_return=create_zero_matrix(row,col);
  for (size_t i=1;i<=row;i++){
    insert_value_without_check(1,i,i,to_return);
  }
  return to_return;
}

/* free the memory allocated by matrix mat */
void free_matrix(matrix* mat) {
  if (mat!=NULL){
    free(mat->start);
    free(mat);
  }
}

/* calculate the dot product */
value dot_product(matrix* r, matrix* v) {
  value ans = 0;
  for (size_t i = 1; i <= r->rows; i++) {
    ans += get_value_without_check(i,1,r) * get_value_without_check(i,1,v);
  }
  return ans;
}

/*prints the matrix*/
void print_matrix(matrix* mat) {
  size_t col = 1;
  size_t row = 1;
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
bool check_boundaries(size_t row, size_t col, matrix* mat) {
  return ((row >= 1) && (col >= 1) && (col <= mat->columns)
	  && (row <= mat->rows));
}

/* Insert a array into the matrix, the array must have the same size as number of total elements in the matrix */
bool insert_array(value arr[], matrix* mat) {
  size_t size = mat->size;
  size_t row = 1;
  size_t col = 1;
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

/* Returns true if matrices a and b look the same */
bool compare_matrices(matrix* a, matrix* b) {
  if ((a->columns != b->columns) || (a->rows != b->rows)) {
    return false;
  }
  
  for(size_t i = 1; i <= a->rows; i++){
    for(size_t j = 1; j <= a->columns; j++){
      value a_val = get_value_without_check(i, j, a);
      value b_val = get_value_without_check(i, j, b);
      if(!(matlib_fabs(a_val - b_val) <= PRECISION)){
	return false;
      }
    }
  }
  return true;
}

/* Return true if the matrix are the same */
bool is_matrix(matrix* a, matrix* b) {
  return ((a->size == b->size) && (a->rows == b->rows)
	  && (a->columns == b->columns) && (a->start == b->start));
}


/* Inserts value at pos rowm,col in matrix mat */
bool insert_value(value insert,size_t row, size_t col, matrix* mat) {
  if (!check_boundaries(row, col, mat)) {
    return false;
  }
  value* start = mat->start + mat->columns * (row - 1)  + (col - 1);
  *(start) = insert;
  return true;
}

/* As insert_value without check */
void insert_value_without_check(value insert, size_t row, size_t col, matrix* mat) {
  *(mat->start + mat->columns * (row - 1) + (col - 1)) = insert;
}

/* Returns value on location row,col in matrix mat.
 * WARNING: Only returns 0 if outside of the matrix */
value get_value(size_t row, size_t col, matrix* mat) {
  if (!check_boundaries(row, col, mat)) {
    return false;
  }
  return *(mat->start + mat->columns * (row - 1) + (col - 1));
}

/* As get_value without check */
value get_value_without_check(size_t row, size_t col, matrix* mat) {
  return *(mat->start + mat->columns * (row - 1) + (col - 1));
}

/* Adds a and b into c. c=a+b */
bool add_matrices(matrix* a, matrix* b, matrix* c) {
  size_t size;
  size_t i;
  size_t check = a->columns;
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

/* Adds a and b by returning a pointer a matrix with a+b */
matrix* add_matrices_with_return(matrix* a, matrix* b) {
  matrix* c=create_matrix(a->rows,a->columns);
  if (add_matrices(a,b,c)){
    return c;
  }
  else{
    free_matrix(c);
    return NULL;
  }
}


/* Subtract a and b into c. c=a-b */
bool subtract_matrices(matrix* a, matrix* b, matrix* c) {
  size_t size;
  size_t i;
  size_t check = a->columns;
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

/* Subtracts a and b by returning a pointer a matrix with a-b */
matrix* subtract_matrices_with_return(matrix* a, matrix* b) {
  matrix* c=create_matrix(a->rows,a->columns);
  if (subtract_matrices(a,b,c)){
    return c;
  }
  else{
    free_matrix(c);
    return NULL;
  }
}


/* Multiply a and b into c. c=a*b */
bool multiply_matrices(matrix* a, matrix* b, matrix* c) {
  return  multiply_matrices_naive(a,b,c);
}


/* Multiply a and b into c using the naive algorithm. c=a*b */
bool multiply_matrices_naive(matrix* a, matrix* b, matrix* c) {
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
	sum += get_value_without_check(i, j, a)* get_value_without_check(j, k, b);
      }
      insert_value_without_check(sum, i, k, c);
    }
  }
  return true;
}

/* Multiply a and b into c. Uses row-major optimization. c=a*b */
bool multiply_matrices_optimized(matrix* a, matrix* b, matrix* c) {
  if ((a->columns != b->rows) || (a->rows != c->rows)
      || (b->columns != c->columns)) {
    return false;
  }
  matrix* b_trans= transpose_matrix_with_return(b);
  size_t j = 1;
  size_t i = 1;
  size_t k = 1;
  value sum = 0;
  for (; i <= a->rows; i++) {
    k = 1;
    for (; k <= b_trans->rows; k++) {
      sum = 0;
      j = 1;
      for (; j <= b_trans->columns; j++) {
  sum += get_value_without_check(i, j, a)* get_value_without_check(k,j, b_trans);
      }
      insert_value_without_check(sum, i, k, c);
    }
  }
  free_matrix(b_trans);
  return true;
}

/* Multiply a and b using the Strassen algorithm and return a pointer to matrix c. c=a*b */
matrix* strassen_matrices_with_return(matrix* a, matrix* b) {
  matrix* c =create_matrix(a->columns,b->rows);
  if (strassen_matrices(a,b,c)){
    return c;
  }
  else{
    free_matrix(c);
    return NULL;
  }
}


/* Multiply a and b into c using the Strassen algorithm. c=a*b */
bool strassen_matrices(matrix* a, matrix* b, matrix* c) {
 if (a->rows<=512  && b->columns<=512){
    return multiply_matrices_naive(a,b,c);
  }
  if ((a->columns != b->rows) || (a->rows != c->rows)
      || (b->columns != c->columns)) {
    return false;
  }
  matrix* a_corrected;
  matrix* b_corrected;
  matrix* c_corrected;

   /* If any of the matrices are uneven they are filled out with a zero row and/or column
    * to make them even */
  a_corrected=create_zero_matrix(a->rows+a->rows%2,a->columns+a->columns%2);
  insert_sub_matrix(1,a->rows,1,a->columns,a,a_corrected);

  b_corrected=create_zero_matrix(b->rows+b->rows%2,b->columns+b->columns%2);
  insert_sub_matrix(1,b->rows,1,b->columns,b,b_corrected);

  c_corrected=create_zero_matrix(c->rows+c->rows%2,c->columns+c->columns%2);

  matrix* mover=create_matrix(a_corrected->rows/2,a_corrected->columns/2);

  /* a is split into | a11 a12 |
   *                 | a21 a22 | */

  get_sub_matrix(1,a_corrected->rows/2,1,a_corrected->columns/2,a_corrected,mover);
  matrix* a11=matrix_copy(mover);

  get_sub_matrix(a_corrected->rows/2+1,a_corrected->rows,1,a_corrected->columns/2,a_corrected,mover);
  matrix* a21=matrix_copy(mover);

  get_sub_matrix(1,a_corrected->rows/2,a_corrected->columns/2+1,a_corrected->columns,a_corrected,mover);
  matrix* a12=matrix_copy(mover);

  get_sub_matrix(a_corrected->rows/2+1 ,a_corrected->rows,a_corrected->columns/2+1,a_corrected->columns,a_corrected,mover);
  matrix* a22=matrix_copy(mover);

  free_matrix(mover);

  mover=create_matrix(b_corrected->rows/2,b_corrected->columns/2);
  /* b is split into | b11 b12 |
   *                 | b21 b22 | */

  get_sub_matrix(1,b_corrected->rows/2,1,b_corrected->columns/2,b_corrected,mover);
  matrix* b11=matrix_copy(mover);

  get_sub_matrix(b_corrected->rows/2+1,b_corrected->rows,1,b_corrected->columns/2,b_corrected,mover);
  matrix* b21=matrix_copy(mover);

  get_sub_matrix(1,b_corrected->rows/2,b_corrected->columns/2+1,b_corrected->columns,b_corrected,mover);
  matrix* b12=matrix_copy(mover);

  get_sub_matrix(b_corrected->rows/2+1 ,b_corrected->rows,b_corrected->columns/2+1,b_corrected->columns,b_corrected,mover);
  matrix* b22=matrix_copy(mover);

  free_matrix(mover);

  matrix* a11a22=add_matrices_with_return(a11,a22);
  matrix* b11b22=add_matrices_with_return(b11,b22);

  matrix* M1;

  M1=strassen_matrices_with_return(a11a22,b11b22);

  free_matrix(a11a22);
  free_matrix(b11b22);

  matrix* a21a22=add_matrices_with_return(a21,a22);
  matrix* M2;

  M2=strassen_matrices_with_return(a21a22,b11);

  free_matrix(a21a22);

  matrix* b12b22=subtract_matrices_with_return(b12,b22);
  matrix* M3;

    M3=strassen_matrices_with_return(a11,b12b22);

  free_matrix(b12b22);

  matrix* b21b11=subtract_matrices_with_return(b21,b11);
  matrix* M4;

  M4=strassen_matrices_with_return(a22,b21b11);

  free_matrix(b21b11);

  matrix* a11a12=add_matrices_with_return(a11,a12);
  matrix* M5;

  M5=strassen_matrices_with_return(a11a12,b22);

  free_matrix(a11a12);

  matrix* a21a11=subtract_matrices_with_return(a21,a11);
  matrix* b11b12=add_matrices_with_return(b11,b12);
  matrix* M6;

  M6=strassen_matrices_with_return(a21a11,b11b12);

  free_matrix(a21a11);
  free_matrix(b11b12);

  matrix* a12a22=subtract_matrices_with_return(a12,a22);
  matrix* b21b22=add_matrices_with_return(b21,b22);
  matrix* M7;

  M7=strassen_matrices_with_return(a12a22,b21b22);


  free_matrix(a12a22);
  free_matrix(b21b22);

  matrix* M1M4=add_matrices_with_return(M1,M4);
  matrix* M1M4M5=subtract_matrices_with_return(M1M4,M5);
  matrix* C11=add_matrices_with_return(M1M4M5,M7);

  free_matrix(M1M4);
  free_matrix(M1M4M5);

  matrix* C12=add_matrices_with_return(M3,M5);

  matrix* C21=add_matrices_with_return(M2,M4);

  matrix* M1M2=subtract_matrices_with_return(M1,M2);
  matrix* M3M6=add_matrices_with_return(M3,M6);
  matrix* C22=add_matrices_with_return(M1M2,M3M6);

  free_matrix(M1M2);
  free_matrix(M3M6);

  insert_sub_matrix(1,c_corrected->rows/2,1,c_corrected->columns/2,C11,c_corrected);

  insert_sub_matrix(1,c_corrected->rows/2,c_corrected->columns/2+1,c_corrected->columns,C12,c_corrected);

  insert_sub_matrix(c_corrected->rows/2+1,c_corrected->rows,1,c_corrected->columns/2,C21,c_corrected);

  insert_sub_matrix(c_corrected->rows/2+1,c_corrected->rows,c_corrected->columns/2+1,c_corrected->columns,C22,c_corrected);

  free_matrix(a11);
  free_matrix(a12);
  free_matrix(a21);
  free_matrix(a22);

  free_matrix(b11);
  free_matrix(b12);
  free_matrix(b21);
  free_matrix(b22);

  free_matrix(C11);
  free_matrix(C12);
  free_matrix(C21);
  free_matrix(C22);

  free_matrix(M1);
  free_matrix(M2);
  free_matrix(M3);
  free_matrix(M4);
  free_matrix(M5);
  free_matrix(M6);
  free_matrix(M7);

  free_matrix(a_corrected);
  free_matrix(b_corrected);

  insert_sub_matrix(1,c->rows,1,c->columns,c_corrected,c);
  free_matrix(c_corrected);

  return true;
}

#ifdef PARALLEL

/* Multiply a and b using the Strassen algorithm in parallel, returns a pointer to c. c=a*b */
matrix* strassen_matrices_parallel_with_return(matrix* a, matrix* b) {
  matrix* c =create_matrix(a->columns,b->rows);
  if (strassen_matrices_parallel(a,b,c)){
    return c;
  }
  else{
    free_matrix(c);
    return NULL;
  }
}

/* Help function to strassen parallel */
void *calculation_one(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* a11=a->one;
  matrix* a22=a->two;
  matrix* b11=a->three;
  matrix* b22=a->four;
  matrix* M1;
  matrix* a11a22=add_matrices_with_return(a11,a22);
  matrix* b11b22=add_matrices_with_return(b11,b22);

  pthread_mutex_lock(&lock);
  bool create_thread=true;
  if (*thread_counter==number_of_cores){
    create_thread=false;
  }
  if (!create_thread){
    pthread_mutex_unlock(&lock);
    M1=strassen_matrices_with_return(a11a22,b11b22);
  }
  else{
    *thread_counter=*thread_counter+1;
    pthread_mutex_unlock(&lock);
    M1=strassen_matrices_parallel_with_return(a11a22,b11b22);
    pthread_mutex_lock(&lock);
    *thread_counter=*thread_counter-1;
    pthread_mutex_unlock(&lock);
  }

  free_matrix(a11a22);
  free_matrix(b11b22);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M1);
}

/* Help function to strassen parallel */
void *calculation_two(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* a21=a->one;
  matrix* a22=a->two;
  matrix* b11=a->three;

  matrix* a21a22=add_matrices_with_return(a21,a22);
  matrix* M2;

  pthread_mutex_lock(&lock);
  bool create_thread=true;
  if (*thread_counter==number_of_cores){
    create_thread=false;
  }
  if (!create_thread){
    pthread_mutex_unlock(&lock);
    M2=strassen_matrices_with_return(a21a22,b11);
  }
  else{
    *thread_counter=*thread_counter+1;
    pthread_mutex_unlock(&lock);
    M2=strassen_matrices_parallel_with_return(a21a22,b11);
    pthread_mutex_lock(&lock);
    *thread_counter=*thread_counter-1;
    pthread_mutex_unlock(&lock);
  }

  free_matrix(a21a22);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M2);
}

/* Help function to strassen parallel */
void *calculation_three(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* b12=a->one;
  matrix* b22=a->two;
  matrix* a11=a->three;

  matrix* b12b22=subtract_matrices_with_return(b12,b22);
  matrix* M3;

  pthread_mutex_lock(&lock);
  bool create_thread=true;
  if (*thread_counter==number_of_cores){
    create_thread=false;
  }
  if (!create_thread){
    pthread_mutex_unlock(&lock);
    M3=strassen_matrices_with_return(a11,b12b22);
  }
  else{
    *thread_counter=*thread_counter+1;
    pthread_mutex_unlock(&lock);
    M3=strassen_matrices_parallel_with_return(a11,b12b22);
    pthread_mutex_lock(&lock);
    *thread_counter=*thread_counter-1;
    pthread_mutex_unlock(&lock);
  }

  free_matrix(b12b22);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M3);
}

/* Help function to strassen parallel */
void *calculation_four(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* b21=a->one;
  matrix* b11=a->two;
  matrix* a22=a->three;
  matrix* b21b11=subtract_matrices_with_return(b21,b11);
  matrix* M4;

    pthread_mutex_lock(&lock);
    bool create_thread=true;
    if (*thread_counter==number_of_cores){
      create_thread=false;
    }
    if (!create_thread){
      pthread_mutex_unlock(&lock);
      M4=strassen_matrices_with_return(a22,b21b11);
    }
    else{
      *thread_counter=*thread_counter+1;
      pthread_mutex_unlock(&lock);
      M4=strassen_matrices_parallel_with_return(a22,b21b11);
      pthread_mutex_lock(&lock);
      *thread_counter=*thread_counter-1;
      pthread_mutex_unlock(&lock);
    }

  free_matrix(b21b11);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M4);
}

/* Help function to strassen parallel */
void *calculation_five(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* a11=a->one;
  matrix* a12=a->two;
  matrix* b22=a->three;

  matrix* a11a12=add_matrices_with_return(a11,a12);
  matrix* M5;

    pthread_mutex_lock(&lock);
    bool create_thread=true;
    if (*thread_counter==number_of_cores){
      create_thread=false;
    }
    if (!create_thread){
      pthread_mutex_unlock(&lock);
      M5=strassen_matrices_with_return(a11a12,b22);
    }
    else{
      *thread_counter=*thread_counter+1;
      pthread_mutex_unlock(&lock);
      M5=strassen_matrices_parallel_with_return(a11a12,b22);
      pthread_mutex_lock(&lock);
      *thread_counter=*thread_counter-1;
      pthread_mutex_unlock(&lock);
    }


  free_matrix(a11a12);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M5);
}

/* Help function to strassen parallel */
void *calculation_six(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* a21=a->one;
  matrix* a11=a->two;
  matrix* b11=a->three;
  matrix* b12=a->four;
  matrix* a21a11=subtract_matrices_with_return(a21,a11);
  matrix* b11b12=add_matrices_with_return(b11,b12);
  matrix* M6;

    pthread_mutex_lock(&lock);
    bool create_thread=true;
    if (*thread_counter==number_of_cores){
      create_thread=false;
    }
    if (!create_thread){
      pthread_mutex_unlock(&lock);
      M6=strassen_matrices_with_return(a21a11,b11b12);
    }
    else{
      *thread_counter=*thread_counter+1;
      pthread_mutex_unlock(&lock);
      M6=strassen_matrices_parallel_with_return(a21a11,b11b12);
      pthread_mutex_lock(&lock);
      *thread_counter=*thread_counter-1;
      pthread_mutex_unlock(&lock);
    }

  free_matrix(a21a11);
  free_matrix(b11b12);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M6);
}

/* Help function to strassen parallel */
void *calculation_seven(void* arg){
  matrices* a = (matrices*)(arg);
  matrix* a12=a->one;
  matrix* a22=a->two;
  matrix* b21=a->three;
  matrix* b22=a->four;
  matrix* a12a22=subtract_matrices_with_return(a12,a22);
  matrix* b21b22=add_matrices_with_return(b21,b22);
  matrix* M7;

  pthread_mutex_lock(&lock);
  bool create_thread=true;
  if (*thread_counter==number_of_cores){
    create_thread=false;
  }
  if (!create_thread){
    pthread_mutex_unlock(&lock);
    M7=strassen_matrices_with_return(a12a22,b21b22);
  }
  else{
    *thread_counter=*thread_counter+1;
    pthread_mutex_unlock(&lock);
    M7=strassen_matrices_parallel_with_return(a12a22,b21b22);
    pthread_mutex_lock(&lock);
    *thread_counter=*thread_counter-1;
    pthread_mutex_unlock(&lock);
  }

  free_matrix(a12a22);
  free_matrix(b21b22);
  pthread_mutex_lock(&lock);
  *thread_counter = *thread_counter - 1;
  pthread_mutex_unlock(&lock);
  pthread_exit(M7);
}

/* Multiply a and b into c using the Strassen algorithm. c=a*b */
bool strassen_matrices_parallel(matrix* a, matrix* b, matrix* c) {
  if (a->rows<=512  && b->columns<=512){
    return multiply_matrices_naive(a,b,c);
  }
  if ((a->columns != b->rows) || (a->rows != c->rows)
      || (b->columns != c->columns)) {
    return false;
  }
  matrix* a_corrected;
  matrix* b_corrected;
  matrix* c_corrected;

   /* If any of the matrices are uneven they are filled out with a zero row and/or column
    * to make them even */
  a_corrected=create_zero_matrix(a->rows+a->rows%2,a->columns+a->columns%2);
  insert_sub_matrix(1,a->rows,1,a->columns,a,a_corrected);

  b_corrected=create_zero_matrix(b->rows+b->rows%2,b->columns+b->columns%2);
  insert_sub_matrix(1,b->rows,1,b->columns,b,b_corrected);

  c_corrected=create_zero_matrix(c->rows+c->rows%2,c->columns+c->columns%2);

  matrix* mover=create_matrix(a_corrected->rows/2,a_corrected->columns/2);

  /* a is split into | a11 a12 |
   *                 | a21 a22 | */

  get_sub_matrix(1,a_corrected->rows/2,1,a_corrected->columns/2,a_corrected,mover);
  matrix* a11=matrix_copy(mover);

  get_sub_matrix(a_corrected->rows/2+1,a_corrected->rows,1,a_corrected->columns/2,a_corrected,mover);
  matrix* a21=matrix_copy(mover);

  get_sub_matrix(1,a_corrected->rows/2,a_corrected->columns/2+1,a_corrected->columns,a_corrected,mover);
  matrix* a12=matrix_copy(mover);

  get_sub_matrix(a_corrected->rows/2+1 ,a_corrected->rows,a_corrected->columns/2+1,a_corrected->columns,a_corrected,mover);
  matrix* a22=matrix_copy(mover);
  free_matrix(mover);

  mover=create_matrix(b_corrected->rows/2,b_corrected->columns/2);
  /* b is split into | b11 b12 |
   *                 | b21 b22 | */

  get_sub_matrix(1,b_corrected->rows/2,1,b_corrected->columns/2,b_corrected,mover);
  matrix* b11=matrix_copy(mover);

  get_sub_matrix(b_corrected->rows/2+1,b_corrected->rows,1,b_corrected->columns/2,b_corrected,mover);
  matrix* b21=matrix_copy(mover);

  get_sub_matrix(1,b_corrected->rows/2,b_corrected->columns/2+1,b_corrected->columns,b_corrected,mover);
  matrix* b12=matrix_copy(mover);

  get_sub_matrix(b_corrected->rows/2+1 ,b_corrected->rows,b_corrected->columns/2+1,b_corrected->columns,b_corrected,mover);
  matrix* b22=matrix_copy(mover);

  free_matrix(mover);

  matrix* M1;
  matrix* M2;
  matrix* M3;
  matrix* M4;
  matrix* M5;
  matrix* M6;
  matrix* M7;

  bool M1_wait;
  bool M2_wait;
  bool M3_wait;
  bool M4_wait;
  bool M5_wait;
  bool M6_wait;
  bool M7_wait;
  pthread_t thread1;
  matrices* thread1_data;
  pthread_t thread2;
  matrices* thread2_data;
  pthread_t thread3;
  matrices* thread3_data;
  pthread_t thread4;
  matrices* thread4_data;
  pthread_t thread5;
  matrices* thread5_data;
  pthread_t thread6;
  matrices* thread6_data;
  pthread_t thread7;
  matrices* thread7_data;

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);
    matrix* a11a22 = add_matrices_with_return(a11, a22);
    matrix* b11b22 = add_matrices_with_return(b11, b22);
    M1 = strassen_matrices_with_return(a11a22, b11b22);
    free_matrix(a11a22);
    free_matrix(b11b22);
    M1_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread one */
    thread1_data = (matrices *) malloc(sizeof(matrices));
    thread1_data->one = a11;
    thread1_data->two = a22;
    thread1_data->three = b11;
    thread1_data->four = b22;

    pthread_create(&thread1, NULL, calculation_one, (void *) thread1_data);
    M1_wait = true;
  }

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);

    matrix* a21a22 = add_matrices_with_return(a21, a22);

    M2 = strassen_matrices_with_return(a21a22, b11);

    free_matrix(a21a22);
    M2_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread two */
    thread2_data = (matrices *) malloc(sizeof(matrices));
    thread2_data->one = a21;
    thread2_data->two = a22;
    thread2_data->three = b11;

    pthread_create(&thread2, NULL, calculation_two, (void *) thread2_data);
    M2_wait = true;
  }

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);

    matrix* b12b22 = subtract_matrices_with_return(b12, b22);

    M3 = strassen_matrices_with_return(a11, b12b22);

    free_matrix(b12b22);
    M3_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread three */
    thread3_data = (matrices *) malloc(sizeof(matrices));
    thread3_data->one = b12;
    thread3_data->two = b22;
    thread3_data->three = a11;

    pthread_create(&thread3, NULL, calculation_three, (void *) thread3_data);
    M3_wait = true;
  }

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);

    matrix* b21b11 = subtract_matrices_with_return(b21, b11);

    M4 = strassen_matrices_with_return(a22, b21b11);

    free_matrix(b21b11);
    M4_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread four */
    thread4_data = (matrices *) malloc(sizeof(matrices));
    thread4_data->one = b21;
    thread4_data->two = b11;
    thread4_data->three = a22;

    pthread_create(&thread4, NULL, calculation_four, (void *) thread4_data);
    M4_wait = true;
  }

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);

    matrix* a11a12 = add_matrices_with_return(a11, a12);

    M5 = strassen_matrices_with_return(a11a12, b22);

    free_matrix(a11a12);
    M5_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread five */
    thread5_data = (matrices *) malloc(sizeof(matrices));
    thread5_data->one = a11;
    thread5_data->two = a12;
    thread5_data->three = b22;

    pthread_create(&thread5, NULL, calculation_five, (void *) thread5_data);
    M5_wait = true;
  }

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);

    matrix* a21a11 = subtract_matrices_with_return(a21, a11);
    matrix* b11b12 = add_matrices_with_return(b11, b12);

    M6 = strassen_matrices_with_return(a21a11, b11b12);

    free_matrix(a21a11);
    free_matrix(b11b12);
    M6_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread six */
    thread6_data = (matrices *) malloc(sizeof(matrices));
    thread6_data->one = a21;
    thread6_data->two = a11;
    thread6_data->three = b11;
    thread6_data->four = b12;

    pthread_create(&thread6, NULL, calculation_six, (void *) thread6_data);
    M6_wait = true;
  }

  pthread_mutex_lock(&lock);
  if (*thread_counter == number_of_cores) {
    pthread_mutex_unlock(&lock);

    matrix* a12a22 = subtract_matrices_with_return(a12, a22);
    matrix* b21b22 = add_matrices_with_return(b21, b22);

    M7 = strassen_matrices_with_return(a12a22, b21b22);

    free_matrix(a12a22);
    free_matrix(b21b22);
    M7_wait = false;
  }
  else {
    *thread_counter = *thread_counter + 1;
    pthread_mutex_unlock(&lock);

    /* Execute thread seven */
    thread7_data = (matrices *) malloc(sizeof(matrices));
    thread7_data->one = a12;
    thread7_data->two = a22;
    thread7_data->three = b21;
    thread7_data->four = b22;

    pthread_create(&thread7, NULL, calculation_seven, (void *) thread7_data);
    M7_wait = true;
  }

  /* Wait for all threads free all memory */
  if (M1_wait){
  pthread_join(thread1, &M1);
  free(thread1_data);
  }
  if (M2_wait){
  pthread_join(thread2, &M2);
  free(thread2_data);
  }
  if (M3_wait){
  pthread_join(thread3, &M3);
  free(thread3_data);
  }
  if (M4_wait){
  pthread_join(thread4, &M4);
  free(thread4_data);
  }
  if (M5_wait){
  pthread_join(thread5, &M5);
  free(thread5_data);
  }
  if (M6_wait){
  pthread_join(thread6, &M6);
  free(thread6_data);
  }
  if (M7_wait){
  pthread_join(thread7, &M7);
  free(thread7_data);
  }
  matrix* M1M4=add_matrices_with_return(M1,M4);
  matrix* M1M4M5=subtract_matrices_with_return(M1M4,M5);
  matrix* C11=add_matrices_with_return(M1M4M5,M7);

  free_matrix(M1M4);
  free_matrix(M1M4M5);

  matrix* C12=add_matrices_with_return(M3,M5);

  matrix* C21=add_matrices_with_return(M2,M4);

  matrix* M1M2=subtract_matrices_with_return(M1,M2);
  matrix* M3M6=add_matrices_with_return(M3,M6);
  matrix* C22=add_matrices_with_return(M1M2,M3M6);

  free_matrix(M1M2);
  free_matrix(M3M6);

  insert_sub_matrix(1,c_corrected->rows/2,1,c_corrected->columns/2,C11,c_corrected);

  insert_sub_matrix(1,c_corrected->rows/2,c_corrected->columns/2+1,c_corrected->columns,C12,c_corrected);

  insert_sub_matrix(c_corrected->rows/2+1,c_corrected->rows,1,c_corrected->columns/2,C21,c_corrected);

  insert_sub_matrix(c_corrected->rows/2+1,c_corrected->rows,c_corrected->columns/2+1,c_corrected->columns,C22,c_corrected);

  free_matrix(a11);
  free_matrix(a12);
  free_matrix(a21);
  free_matrix(a22);

  free_matrix(b11);
  free_matrix(b12);
  free_matrix(b21);
  free_matrix(b22);

  free_matrix(C11);
  free_matrix(C12);
  free_matrix(C21);
  free_matrix(C22);

  free_matrix(M1);
  free_matrix(M2);
  free_matrix(M3);
  free_matrix(M4);
  free_matrix(M5);
  free_matrix(M6);
  free_matrix(M7);

  free_matrix(a_corrected);
  free_matrix(b_corrected);

  insert_sub_matrix(1,c->rows,1,c->columns,c_corrected,c);

  return true;
}

/* Initializes parallel variables */
void initialize_parallelization(){
  pthread_mutex_init(&lock, NULL);
  thread_counter=(int*)malloc(sizeof(int));
  *thread_counter=0;
}

/* Free parallel variables */
void deinitialize_parallelization(){
  free(thread_counter);
  pthread_mutex_destroy(&lock);
}

#endif


/* Multiply a and b by returning a pointer to a new matrix with a*b*/
matrix* multiply_matrices_with_return(matrix* a, matrix* b) {
  if ((a->columns != b->rows)) {
    return NULL;
  }
  matrix* c = create_matrix(a->rows, b->columns);
  multiply_matrices(a, b, c);
  return c;
}

/* Returns the determinant of matrix a */
value get_determinant(matrix* a){
  matrix* u=create_matrix(a->rows,a->columns);
  matrix* l=create_matrix(a->rows,a->columns);
  if (!crout(a,l,u)){
    free_matrix(u);
    free_matrix(l);
    return 0;
  }
  matrix* b=create_matrix(1,a->columns);
  matrix* c=create_matrix(1,a->columns);
  get_diagonal(l,b);
  get_diagonal(u,c);
  value temp=product_of_row(1,b)*product_of_row(1,c);
  free_matrix(l);
  free_matrix(u);
  free_matrix(b);
  free_matrix(c);
  return temp;
}

/* Calculates the inverse of a and puts it into c */
bool get_inverse(matrix* a, matrix* c) {
  if (a->columns != c->columns || a->rows != c->rows) {
    return false;
  }
  matrix* u = create_matrix(a->rows, a->columns);
  matrix* l = create_matrix(a->rows, a->columns);
  if (!crout(a, l, u)) {
    free_matrix(u);
    free_matrix(l);
    return false;
  }
  matrix* x = create_matrix(a->rows, 1);
  matrix* b = create_matrix(a->rows, 1);

  /* Solve for each column */
  for (size_t i = 1; i <= a->columns; i++) {
    /* Fill the b vector */
    for (size_t j = 1; j <= a->columns; j++) {
      if (j == i) {
        insert_value(1.0, j, 1, b);
      }
      else {
        insert_value(0.0, j, 1, b);
      }
    }
    forward_backward(l, u, x, b);
    insert_column_vector(i, x, c);
  }
  free_matrix(u);
  free_matrix(l);
  free_matrix(x);
  free_matrix(b);
  return true;
}

matrix* get_inverse_of_2x2_with_return(matrix* a){
  matrix* b=create_matrix(2,2);
  if (get_inverse_of_2x2(a,b)){
    return b;
  }
  else{
    free_matrix(b);
    return NULL;
  }
}

bool get_inverse_of_2x2(matrix* a,matrix* b){
  if (a->columns!=2||a->rows!=2||b->columns!=2||b->rows!=2){
    return false;
  }
  insert_value(get_value(1,1,a),2,2,b);
  insert_value(-get_value(1,2,a),1,2,b);
  insert_value(-get_value(2,1,a),2,1,b);
  insert_value(get_value(2,2,a),1,1,b);
  value temp=get_value(1,1,a)*get_value(2,2,a)-get_value(2,1,a)*get_value(1,2,a);
  if (temp==0){
    return false;
  }
  temp=1/temp;
  multiply_matrix_with_scalar(temp,b);
  return true;
}

/* Solves Ax=B */
bool solve_linear(matrix* a, matrix* x, matrix* b){
  matrix* u=create_matrix(a->rows,a->columns);
  matrix* l=create_matrix(a->rows,a->columns);
  if (!crout(a,l,u)){
    free_matrix(u);
    free_matrix(l);
    return false;
  }
  forward_backward(l,u,x,b);
  free_matrix(u);
  free_matrix(l);
  return true;
}

/* Solves ax=b by returning a pointer to x */
matrix* solve_linear_with_return(matrix* a,matrix *b){
  matrix* x = create_matrix(b->rows,b->columns);
  if (solve_linear(a,x,b)){
  return x;
  }
  else{
    free_matrix(x);
    return NULL;
  }
}

/* Crout algorithm to divide matrix a into l and u that holds a=lu */
bool crout(matrix* a, matrix* l, matrix* u) {
  if (a->rows != a->columns) {
    return false;
  }
  size_t check = a->rows;
  if (check != l->rows || check != u->rows) {
    return false;
  }
  check = a->columns;
  if (check != l->columns || check != u->columns) {
    return false;
  }
  size_t i, j, k;
  double sum = 0;
  size_t n = a->columns;
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

  size_t i = 1;
  size_t j = 1;
  size_t n = l->rows;
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

/* If no solution can be found with solve_linear, this function finds the closest one */
void least_square(matrix* a, matrix* x, matrix* b) {
  matrix* trans_a = create_matrix(a->columns, a->rows);
  transpose_matrix(a, trans_a); 

  matrix* lhs = create_matrix(trans_a->rows, a->columns);
  matrix* rhs = create_matrix(trans_a->rows, b->columns);
  
  multiply_matrices(trans_a, a, lhs);
  multiply_matrices(trans_a, b, rhs);
  gauss_jordan_solver(lhs, x, rhs);

  free_matrix(trans_a);
  free_matrix(lhs);
  free_matrix(rhs);
}

/* Gauss eliminates the matrix a */
bool gauss_jordan(matrix* a) {
  if(a->rows != a->columns) return false;

  for (size_t k = 1; k <= min(a->rows, a->columns); k++) {
    size_t pivot = largest_element_in_column_index(k,k, a);
    if (get_value(pivot, k, a) == 0) {
      pivot=smallest_element_in_column_index(k,k, a);
      if (get_value(pivot, k, a) == 0){
        return false;
      }
      multiply_row_with_scalar(-1,pivot,a);
    }
    switch_rows(k, pivot, a);
    for (size_t i = k + 1; i <= a->rows; i++) {
      for (size_t j = k + 1; j <= a->columns; j++) {
        value temp1=get_value(i,j,a)-get_value(k,j,a)*(get_value(i,k,a)/get_value(k,k,a));
        insert_value(temp1,i,j,a);
      }
      insert_value(0,i,k,a);
    }
  }
  return true;
}

/* Solves the system of linear equations using gauss jordan */
bool gauss_jordan_solver(matrix* a,matrix* x,matrix* b) {
  if (a->columns!=x->rows||a->rows!=b->rows||x->columns!=b->columns||a->columns!=a->rows){
    return false;
  }
  matrix* A=matrix_copy(a);
  matrix* B=matrix_copy(b);
  value multiplier;
  value temp;
  for(size_t j=1;j<=A->columns-1;j++){

    size_t pivot = largest_element_in_column_index(j,j, A);
    if (get_value(pivot, j, A) == 0) {
      pivot=smallest_element_in_column_index(j,j, A);
      if (get_value(pivot, j, A) == 0){
        return false;
      }
      multiply_row_with_scalar(-1,pivot,A);
      multiply_row_with_scalar(-1,pivot,B);
    }
    switch_rows(j, pivot, A);
    switch_rows(j, pivot, B);
    for (size_t i=j+1;i<=A->columns;i++){
      multiplier=get_value_without_check(i,j,A)/get_value_without_check(j,j,A);
      for (size_t k=j+1;k<=A->columns;k++){
        temp=get_value_without_check(i,k,A)-multiplier*get_value_without_check(j,k,A);
        insert_value_without_check(temp,i,k,A);
      }
      temp=get_value_without_check(i,1,B)-multiplier*get_value_without_check(j,1,B);
      insert_value_without_check(temp,i,1,B);
      insert_value(0,i,j,A);
    }

  }
  for (size_t i=A->rows;i>=1;i--){
    insert_value_without_check(get_value_without_check(i,1,B),i,1,x);
    for (size_t j=i+1;j<=A->columns;j++){
      temp=get_value_without_check(i,1,x)-get_value_without_check(i,j,A)*get_value_without_check(j,1,x);
      insert_value_without_check(temp,i,1,x);
    }
    divide_row_with_scalar(get_value_without_check(i,i,A),i,x);
  }
  free_matrix(A);
  free_matrix(B);
  return true;
}

/* Returns a matrix with only pivots elements from a  */
matrix* get_matrix_with_only_pivots(matrix* a){
  matrix* temp=create_matrix(a->rows,a->columns);
  matrix* temp_vector=create_matrix(1,a->columns);
  size_t pivot=0;
  for (size_t i=1;i<=a->rows;i++){
    for (size_t j=1;j<=a->columns;j++){
      if (get_value(i,j,a)!=0){
        pivot++;
        get_row_vector(i,a,temp_vector);
        insert_row_vector(pivot,temp_vector,temp);
        break;
      }
    }
  }

  matrix* to_return=create_matrix(pivot,a->columns);
  get_sub_matrix(1,pivot,1,a->columns,temp,to_return);
  free_matrix(temp);
  free_matrix(temp_vector);
  return to_return;
}

/* Returns the lowest of the two values */
value min(value a,value b){
  if (a<b){
    return a;
  }
  else{
    return b;
  }
}

/* Returns on which row the largest element in the column is after start */
size_t largest_element_in_column_index(size_t column,size_t start,matrix* a){
  if (column > a->columns || column < 1) {
    return false;
  }
  value max = get_value(start, column, a);
  value temp = 0;
  size_t index = start;
  for (size_t i =start+1; i <= a->rows; i++) {
    temp = get_value(i, column, a);
    if (temp > max) {
      max = temp;
      index = i;
    }
  }
  return index;
}

/* Returns on which row the smallest element in the column is after start */
size_t smallest_element_in_column_index(size_t column,size_t start,matrix* a){
  if (column > a->columns || column < 1) {
    return false;
  }
  value min = get_value(start, column, a);
  value temp = 0;
  size_t index = start;
  for (size_t i = start+1; i <= a->rows; i++) {
    temp = get_value(i, column, a);
    if (temp < min) {
      min = temp;
      index = i;
    }
  }
  return index;
}

/* Returns on which row the first nonezero element is in the column is after start returns -1
 * if no nonezero element is found */
size_t first_nonezero_in_column_index(size_t column, size_t start, matrix* a) {
  if (column > a->columns || column < 1) {
    return false;
  }
  for (size_t i = start; i <= a->rows; i++) {
    if ( 0!=get_value(i, column, a)) {
      return i;
    }
  }
  return -1;
}

/* Returns on which column the first nonezero element is in the column is after start returns 0
 * if no nonezero element is found */
size_t first_nonezero_in_row_index(size_t row,size_t start, matrix* a) {
  if (row > a->rows || row < 1) {
    return false;
  }
  for (size_t i = start; i <= a->columns; i++) {
    if (0 != get_value(row, i, a)) {
      return i;
    }
  }
  return 0;
}

/* Adds each element in row1 and row 2 and puts the result on row2 */
void add_rows(size_t row1, size_t row2, matrix* a) {
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
  size_t row = 1;
  size_t col = 1;
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

/* Transposes matrix a by returning a pointer to a:s transpose */
matrix* transpose_matrix_with_return(matrix* a){
  matrix* b=create_matrix(a->columns,a->rows);
  transpose_matrix(a,b);
  return b;
}

/* Return the sum of a row in matrix mat */
value sum_of_row(size_t row, matrix* mat) {
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
value sum_of_column(size_t column, matrix* mat) {
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

/* Return the product of a row in matrix mat */
value product_of_row(size_t row, matrix* mat) {
  if (!check_boundaries(row, 1, mat)) {
    return false;
  }
  size_t i = 0;
  value *start = mat->start + (row - 1) * mat->columns;
  value to_return = 1;
  for (; i < mat->columns; i++) {
    to_return *= *(start + i);
  }
  return to_return;
}

/* Return the product of a column in matrix mat */
value product_of_column(size_t column, matrix* mat) {
  if (!check_boundaries(1, column, mat)) {
    return false;
  }
  size_t i = 0;
  value *start = mat->start + (column - 1);
  value to_return = 1;
  for (; i < mat->rows; i++) {
    to_return *= *(start + i * mat->columns);
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
void divide_matrix_with_scalar(value scal, matrix* mat)
{
  if (scal==0){
    return;
  }
  size_t size = mat->size;
  size_t i = 0;
  for (; i < size; i++)
  {
    *(mat->start + i) /= scal;
  }
}

/* Multiplies a row with a scalar */
void multiply_row_with_scalar(value scal, size_t row, matrix* mat) {
  value* start = mat->start + (row - 1) * mat->columns;
  size_t i = 0;
  for (; i < mat->columns; i++) {
    *(start + i) *= scal;
  }
}

/* Divides a row with a scalar */
void divide_row_with_scalar(value scal, size_t row, matrix* mat) {
  if (scal==0){
    return;
  }
  value* start = mat->start + (row - 1) * mat->columns;
  size_t i = 0;
  for (; i < mat->columns; i++) {
    *(start + i) /= scal;
  }
}

/* Multiplies a column with a scalar */
void multiply_column_with_scalar(value scal, size_t col, matrix* mat) {
  value* start = mat->start + (col - 1);
  size_t i = 0;
  size_t step = mat->rows ;
  for (; i < mat->rows; i++){
    *(start + i * step) *= scal;
  }
}

/* Divides a column with a scalar */
void divide_column_with_scalar(value scal, size_t col, matrix* mat){
  if (scal==0){
    return;
  }
  value* start = mat->start + (col - 1);
  size_t i = 0;
  size_t step = mat->rows ;
  for (; i < mat->rows; i++){
    *(start + i * step) /= scal;
  }
}

/* Takes row vector from matrix a and puts it into b which also is a row vector
 *however get_sub_matrix should be faster */
bool get_row_vector(size_t row, matrix* a, matrix* b){
  if (!check_boundaries(row, 1, a)) {
    return false;
  }
  if (b->rows != 1 || b->columns != a->columns){
    return false;
  }
  size_t i = 0;
  value *start = a->start + (row - 1) * a->columns;
  for (; i < a->columns; i++) {
    *(b->start + i ) = *(start + i);
  }
  return true;
}

/* Returns row vector row from matrix a with a pointer to a matrix */
matrix* get_row_vector_with_return(size_t row,matrix* a){
  matrix* b=create_matrix(1, a->columns);
  if(get_row_vector(row, a, b)){
    return b;
  }
  else{
    free_matrix(b);
    return NULL;
  }
}


/* Inserts row vector a into b:s row */
bool insert_row_vector(size_t row, matrix* a, matrix* b) {
  if ((a->columns != b->columns) || (a->rows != 1)) {
    return false;
  }
  value *start = b->start + (row - 1) * b->columns;
  size_t number_of_bytes = b->columns * sizeof(value);
  memcpy((void *) start, (void *) (a->start), number_of_bytes);
  return true;
}

/* Switches rows in a */
bool switch_rows(size_t row1, size_t row2, matrix* a) {
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
bool get_column_vector(size_t column, matrix* a, matrix* b) {
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
/* Takes column vector from matrix a and puts it into b which also is a column vector
 *however get_sub_matrix should be faster */
matrix* get_column_vector_with_return(size_t column, matrix* a){
  matrix* b=create_matrix(a->rows,1);
  get_column_vector(column,a,b);
  return b;

}



/* Inserts column vector a into matrix b at position column */
bool insert_column_vector(size_t column, matrix *a, matrix* b) {
  if (a->columns != 1 || b->rows != a->rows) {
    return false;
  }
  
  for(size_t i = 1; i <= b->rows; i++){
    value val = get_value_without_check(i, 1, a);
    insert_value_without_check(val, i, column, b);
  }

  return true;
}

/* Takes the submatrix defined by start_row,end_row,start_col,end_col and put it into matrix b */
bool get_sub_matrix(size_t start_row, size_t end_row, size_t start_col, size_t end_col, matrix* a, matrix* b) {
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

  for(size_t i = 0; i < num_rows; i++){
    void* to = b->start + b_row_size * i;
    void* from = a->start + offset + a_row_size * i;
    memcpy(to, from, bytes_per_row);
  }

  return true;
}

/* inserts the submatrix defined by start_row,end_row,start_col,end_col and put it into matrix b */
bool insert_sub_matrix(size_t start_row, size_t end_row, size_t start_col, size_t end_col, matrix* b, matrix* a) {
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

  for(size_t i = 0; i < num_rows; i++){
    void* from = b->start + b_row_size * i;
    void* to = a->start + offset + a_row_size * i;
    memcpy(to, from, bytes_per_row);
  }

  return true;
}

/* Copy and return new matrix. */
matrix* matrix_copy(matrix* source) {
  matrix* m = create_matrix(source->rows,source->columns);
  memcpy(m->start,source->start,source->size * sizeof(value));
  return m;
}

/* Copies all the data from matrix A into matrix B */
void matrix_copy_data(matrix* a, matrix* b) {
  if ((a->columns!=b->columns ||a->rows!=a->rows)){
    return;
  }
  size_t number_of_bytes=a->size*sizeof(value);
  memcpy((void *) (b->start), (void *) (a->start), number_of_bytes);
}

/* checks if all elements in a matrix is equal to zero */
bool is_zero_matrix(matrix* v) {
  for (size_t i = 1; i <= v->rows; i++) {
    for(size_t j = 1; j <= v->columns; j++){
      if (matlib_fabs(get_value_without_check(i,j,v)) > 0.0001) {
	return false;
      }
    }
  }
  return true;
}

/* checks if all elements in a matrix is positive */
bool is_non_negative_matrix(matrix* v) {
  for (size_t i = 1; i <= v->rows; i++) {
    for(size_t j = 1; j <= v->columns; j++){
      if (get_value_without_check(i,j,v) < 0) {
	return false;
      }
    }
  }
  return true;
}

/* checks if all elements along the diagonal in a symmetric matrix is positiv */
bool is_non_negative_diagonal_matrix(matrix* A) {
  for (size_t i = 1; i <= A->rows; i++) {
    if (get_value_without_check(i,i,A) < 0) {
      return false;
    }
  }
  return true;
}

/* Takes the diagonal in a and puts it in b */
bool get_diagonal(matrix* a,matrix* b) {
  if (a->rows != a->columns || b->columns != a->columns) {
    return false;
  }
  for (size_t i = 1; i <= a->columns; i++) {
    insert_value_without_check(get_value_without_check(i, i, a), 1, i, b);
  }
  return true;
}

/* Returns a pointer to a matrix with the derivative of var if the a matrix second order coefficiants */
matrix* derivate_matrix_with_return(size_t var,matrix* a){
  if (a->rows!=a->columns || var>a->columns){
    return NULL;
  }
  matrix* to_return=create_matrix(a->rows,a->columns);
  for (size_t i=1;i<=a->rows;i++){
    for (size_t j=1;j<=a->columns;j++){
      if(i==var &&j==var){
        insert_value(2*get_value(i,j,a),i,j,to_return);
      }
      else if (i==var||j==var){
        insert_value(get_value(i,j,a),i,j,to_return);
      }
      else{
        insert_value(0,i,j,to_return);
      }
    }
  }
  return to_return;
}

/* Transforms a matrix into reduced row echelon form
 * ex: 
 *              (1 5 3)               (1 0 0)
 *      M in =  (1 6 3),    M out =   (0 1 0)
 *              (1 5 4)               (0 0 1)
 *
 *  M doesn't need to be a square matrix
 */
void transform_to_reduced_row_echelon_form(matrix* M) {
  size_t lead = 1;
  size_t i = 1;

  matrix* row1 = create_matrix(1,M->columns);
  matrix* row2 = create_matrix(1,M->columns);
  matrix* row3 = create_matrix(1,M->columns);

  for (size_t r = 1; r <= M->rows; r++){
    if (M->columns+1 <= lead){
      free_matrix(row1);
      free_matrix(row2);
      free_matrix(row3);
      return;
    }
    i = r;
    while (compare_elements(get_value_without_check(i,lead,M),0) == 0){
      i = i + 1;
      if (M->rows+1 == i){
        i = r;
        lead = lead + 1;
        if (M->columns+1 == lead){
          free_matrix(row1);
          free_matrix(row2);
          free_matrix(row3);
          return;
        }
      }
    }
    switch_rows(i,r,M);
    if (compare_elements(get_value_without_check(r,lead,M),0) != 0){
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,M);
    }
    for (i = 1; i <= M->rows; i++){
      if (i != r) {
        get_row_vector(r,M,row1);
        get_row_vector(i,M,row2);
        multiply_matrix_with_scalar(-get_value_without_check(i,lead,M),row1);
        add_matrices(row1,row2,row3);
        insert_row_vector(i,row3,M);
      }
    }
    lead++;
  }
  free_matrix(row1);
  free_matrix(row2);
  free_matrix(row3);
}

/* return true if b contains value a */
bool matrix_contains(value a,matrix* b){
  for (size_t i=1;i<=b->rows;i++){
    for (size_t j=1;j<=b->columns;j++){
      if (get_value(i,j,b)==a){
        return true;
      }
    }
  }
  return false;
}

/* compare two element values */
int compare_elements(value a, value b) {
  value diff = matlib_fabs(a - b);

  if (diff <= PRECISION) {
    /* a == b */
    return 0;
  }else if(a - b < PRECISION && diff > PRECISION){
    /* a < b */    
    return -1;
  }else{
    /* a > b */
    return 1;
  }
}

/* Creates new matrix with zero values */
matrix* get_zero_matrix(size_t rows, size_t columns){
  matrix* zero = create_matrix(rows, columns);
  free(zero->start);
  zero->start = calloc(rows*columns, sizeof(value));

  if(zero->start == NULL){
    return NULL;
  }

  return zero;
}

/* Returns the absolute value of a */
value matlib_fabs(value a){
  if (a==0){
    return 0;
  }
  if (a<0){
    return -a;
  }else{
    return a;
  }

}


