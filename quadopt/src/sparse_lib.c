#include <sparse_lib.h>
#include <math.h>

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
  if (n > 0) {
    S->A = malloc(n*sizeof(value));
    S->rA = malloc(n*sizeof(int));
    S->cA = malloc(n*sizeof(int));
  }

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

/* creates an empty sparse matrix */
sparse_matrix* create_empty_sparse_matrix(int size) {
  /* create sparse matrix */
  sparse_matrix* S = malloc(sizeof(sparse_matrix));
  S->size = size;
  S->A = malloc(size*sizeof(value));
  S->rA = malloc(size*sizeof(int));
  S->cA = malloc(size*sizeof(int));

  return S;
}

/* return number of elements != 0 */
size_t matrix_sparsity(matrix* A) {
  int r, c;
  size_t n = 0;
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
  return true;
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

void transpose_sparse_matrix(sparse_matrix* Ain) {
  int* temp = Ain->rA;
  Ain->rA = Ain->cA;
  Ain->cA = temp;

  int t = Ain->rows;
  Ain->rows = Ain->columns;
  Ain->columns = t;
}

sparse_matrix* transpose_sparse_matrix_with_return(sparse_matrix* Ain) {

  sparse_matrix* S = copy_sparse_matrix(Ain);
  int* temp = S->rA;
  S->rA = S->cA;
  S->cA = temp;

  return S;
}

void print_sparse_matrix(sparse_matrix* S) {
  matrix* M = create_zero_matrix(S->rows, S->columns);
  int i;

  for (i = 0; i < S->size; i++) {
    insert_value_without_check(S->A[i], S->rA[i], S->cA[i], M);
  }

  print_matrix(M);
  free_matrix(M);
}

void free_sparse_matrix(sparse_matrix* S) {
  if (S != NULL) {
    if (S->size > 0) {
      free(S->A);
      free(S->rA);
      free(S->cA);
    }
    free(S);
  }
}


/* solves Ax = b */
bool conjugate_gradient(sparse_matrix* A, matrix* x, matrix* b){
  /* variables */
  value alpha, beta;
  int i;

  /* r0 = b - Ax0 */
  matrix* Ap = multiply_sparse_matrix_matrix(A, x);
  matrix* r = create_matrix(b->rows, b->columns);
  subtract_matrices(b, Ap, r);

  /* p0 = r0 */
  matrix* p = matrix_copy(r);
  matrix* p_temp = matrix_copy(p);

  /* solve */
  value rs_old = dot_product(r, r);

  value rs_new;

  while (true){

    /* clear Ap */
    for (i = 1; i <= Ap->rows; i++) {
      insert_value_without_check(0, i, 1, Ap);
    }

    /* calculate alpha */
    multiply_sparse_matrix_vector(A, p, Ap);
    alpha = rs_old/dot_product(p, Ap);

    /* calculate next x */
    multiply_matrix_with_scalar(alpha, p_temp);
    add_matrices(x, p_temp, x);

    /* calculate next r */
    multiply_matrix_with_scalar(alpha, Ap);
    subtract_matrices(r, Ap, r);

    rs_new = dot_product(r, r);

    /* check if approx. done */
    if (sqrt(rs_new) < 0.000001) { //compare_elements(rs_new, 0) == 0) {
      break;
    }

    /* calculate beta */
    beta = rs_new/rs_old;

    /* calculate next p */
    multiply_matrix_with_scalar(beta, p);
    add_matrices(p, r, p);
    matrix_copy_data(p, p_temp);

    rs_old = rs_new;
  }

  free_matrix(p);
  free_matrix(p_temp);
  free_matrix(r);
  free_matrix(Ap);

  return true;
}
