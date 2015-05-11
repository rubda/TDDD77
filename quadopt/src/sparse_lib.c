#include <sparse_lib.h>

/* creates a sparse matrix out of a normal matrix */
sparse_matrix* create_sparse_matrix(matrix* Ain, int size) {
  int r, c, n = 0,  i = 0;
  value val;

  if (size == -1) {
    /* count non-zero elements */
    for (r = 1; r <= Ain->rows; r++) {
      for (c = 1; c <= Ain->columns; c++) {
        val = get_value_without_check(r, c, Ain);
        if (compare_elements(val, 0) != 0) {
          n++;
        }
      }
    }
  } else {
    n = size;
  }

  /* create sparse matrix */
  sparse_matrix* S = malloc(sizeof(sparse_matrix));
  S->size = n;
  S->rows = Ain->rows;
  S->columns = Ain->columns;
  S->A = malloc(n*sizeof(value));
  S->rA = malloc(n*sizeof(int));
  S->cA = malloc(n*sizeof(int));

  for (r = 1; r <= Ain->rows; r++) {
    for (c = 1; c <= Ain->columns; c++) {
      val = get_value_without_check(r, c, Ain);
      if (compare_elements(val, 0) != 0) {
        S->A[i] = val;
        S->rA[i] = r;
        S->cA[i] = c;
        i++;
      }
    }
  }

  return S;
}

/* return number of elements != 0 */
int matrix_sparsity(matrix* A) {
  int r, c, n = 0;
  value val;
  for (r = 1; r <= A->rows; r++) {
    for (c = 1; c <= A->columns; c++) {
      val = get_value_without_check(r, c, A);
      if (compare_elements(val, 0) != 0) {
        n++;
      }
    }
  }
  return n;
}

bool multiply_sparse_matrix_vector(sparse_matrix* A, matrix* x, matrix* Ax) {
  int i;
  value temp1, temp2;
  //matrix* Ax = create_zero_matrix(x->rows, x->columns);

  /* multiply */
  for (i = 0; i < A->size; i++) {
    temp1 = get_value_without_check(A->cA[i], 1, x);
    temp1 *= A->A[i];
    temp2 = temp1 + get_value_without_check(A->rA[i], 1, Ax);
    insert_value_without_check(temp2, A->rA[i], 1, Ax);    
  }
}

matrix* multiply_sparse_matrix_matrix(sparse_matrix* A, matrix* B) {
  int i, c;
  value temp1, temp2;
  matrix* AB = create_zero_matrix(A->rows, B->columns);

  /* multiply */
  for (c = 1; c <= B->columns; c++) {
    for (i = 0; i < A->size; i++) {
      temp1 = get_value_without_check(A->cA[i], c, B);
      temp1 *= A->A[i];
      temp2 = get_value_without_check(A->rA[i], c, AB) + temp1;
      insert_value_without_check(temp2, A->rA[i], c, AB);      
    }
  }

  return AB;
}


sparse_matrix* copy_sparse_matrix(sparse_matrix* Ain) {
  sparse_matrix* S = malloc(sizeof(sparse_matrix));
  S->size = Ain->size;
  S->rows = Ain->rows;
  S->columns = Ain->columns;
  S->A = malloc(Ain->size*sizeof(value));
  S->rA = malloc(Ain->size*sizeof(int));
  S->cA = malloc(Ain->size*sizeof(int));

  memcpy(S->A, Ain->A, Ain->size*sizeof(value));
  memcpy(S->rA, Ain->rA, Ain->size*sizeof(int));
  memcpy(S->cA, Ain->cA, Ain->size*sizeof(int));

  return S;
}

bool transpose_sparse_matrix(sparse_matrix* Ain) {
  int* temp = Ain->rA;
  Ain->rA = Ain->cA;
  Ain->cA = temp;
}

sparse_matrix* transpose_sparse_matrix_with_return(sparse_matrix* Ain) {

  sparse_matrix* S = copy_sparse_matrix(Ain);
  int* temp = S->rA;
  S->rA = S->cA;
  S->cA = temp;

  return S;
}

print_sparse_matrix(sparse_matrix* S) {
  matrix* M = create_zero_matrix(S->rows, S->columns);
  int i;

  for (i = 0; i < S->size; i++) {
    insert_value_without_check(S->A[i], S->rA[i], S->cA[i], M);
  }

  print_matrix(M);
  free_matrix(M);
}

void free_sparse_matrix(sparse_matrix* S) {
  free(S->A);
  free(S->rA);
  free(S->cA);
}