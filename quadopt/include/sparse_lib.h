#include <matLib.h>

/* store sparse matrix using COO */
struct sparse_matrix {
  int size;
  int rows;
  int columns;
  value* A;
  int* rA;
  int* cA;
};
typedef struct sparse_matrix sparse_matrix;


/* creates a sparse matrix out of a normal matrix */
sparse_matrix* create_sparse_matrix(matrix* Ain, int size);

bool multiply_sparse_matrix_vector(sparse_matrix* A, matrix* x, matrix* Ax);

matrix* multiply_sparse_matrix_matrix(sparse_matrix* A, matrix* B);

sparse_matrix* copy_sparse_matrix(sparse_matrix* Ain);

bool transpose_sparse_matrix(sparse_matrix* Ain);

sparse_matrix* transpose_sparse_matrix_with_return(sparse_matrix* Ain);

print_sparse_matrix(sparse_matrix* S);