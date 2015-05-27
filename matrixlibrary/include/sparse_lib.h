#ifndef SPARSE_LIB_H
#define SPARSE_LIB_H

#include <matLib.h>

/** Store sparse matrix using COO (coordinate list) */
struct sparse_matrix {
  size_t size;
  size_t rows;
  size_t columns;
  value* A;
  size_t* rA;
  size_t* cA;
};
typedef struct sparse_matrix sparse_matrix;

/** Creates a sparse matrix out of a normal matrix */
sparse_matrix* create_sparse_matrix(matrix* Ain, int size);

/** Creates an empty sparse matrix */
sparse_matrix* create_empty_sparse_matrix(size_t size);

/** Converts sparse matrix to normal matrix */
matrix* sparse_to_normal(sparse_matrix* S);

/** Returns number of elements != 0 */
size_t matrix_sparsity(matrix* A);

/** Multiplies sparse matrix with normal vector, stores result in normal matrix Ax */
bool multiply_sparse_matrix_vector(sparse_matrix* A, matrix* x, matrix* Ax);

/** Multiplies sparse matrix with a normal matrix. Returns a normal matrix. */
matrix* multiply_sparse_matrix_matrix(sparse_matrix* A, matrix* B);

/** Copies a sparse matrix and returns it */
sparse_matrix* copy_sparse_matrix(sparse_matrix* Ain);

/** Transposes input sparse matrix */
void transpose_sparse_matrix(sparse_matrix* Ain);

/** Transposes a sparse matrix and returns it in a new sparse matrix */
sparse_matrix* transpose_sparse_matrix_with_return(sparse_matrix* Ain);

/** Prints sparse matrix */
void print_sparse_matrix(sparse_matrix* S);

/** Frees allocated memory of the sparse matrix */
void free_sparse_matrix(sparse_matrix* S);

/** Solves Ax = b, x should be set to 0 */
/** Is not used, due to not working with MATLAB gate. */
bool conjugate_gradient(sparse_matrix* A, matrix* x, matrix* b);

#endif /* SPARSE_LIB_H */
