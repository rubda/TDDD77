

#include<stdio.h>
#include<matLib.h>


int minimum(int a, int b)
{
    if (a<=b) {
        return a;
    }
    else {
        return b;
    }
}

matrix* guassian_eliminate(matrix* a, matrix*b) {
    int min = minimum(a->rows,a->col);
    for (int k = 0; k < min; k++){

        /* Find the k-th pivot */
        /*
        int i_max = 0;
        int temp = 0, temp_l = 0;
        for (int i = k; i <= a->rows) {
            temp = abs(get_value(i,k,a));
            if (temp > i_max) {
                i_max = temp;
            }
        }
        i_max = 

    }



}
 for k = 1 ... min(m,n):
   Find the k-th pivot:
   i_max  := argmax (i = k ... m, abs(A[i, k]))
   if A[i_max, k] = 0
     error "Matrix is singular!"
   swap rows(k, i_max)
   Do for all rows below pivot:
   for i = k + 1 ... m:
     Do for all remaining elements in current row:
     for j = k + 1 ... n:
       A[i, j]  := A[i, j] - A[k, j] * (A[i, k] / A[k, k])
     Fill lower triangular matrix with zeros:
     A[i, k]  := 0
     */



     /* stubs for shits */
     matrix * lagrange;
     lagrange = create_matrix(A->rows,1);
    bool get_langrange(matrix* A, matrix* lagrange, work_set* nws) {}


     value multiply_row_with_vector(matrix* r, matrix* v) {
        //TODO check
        value ans = 0;
        for (int i = 0; i < r->rows) {

            //TODO access the memory directly might be faster
            ans += get_value(i,1,r) * get_value(i,1,v);
        }
        return ans;
     }

     /* just a simple structure for storing different sets */
     struct work_set {
        int count;
        int* data;
     };

     /* calculates step for active set-method */
     value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws) {
        matrix* ai, ati;
        value bi, nom, temp_step, step = 1;

        for (int i = 0; i < nws->count; i++) {
            get_row_vector(nws->data[i],ati);
            nom = multiply_row_with_vector(ati,p);
            if (nom < 0) {
                bi = get_value(nws->data[i],1,B);
                temp_step = (bi - multiply_row_with_vector(ati,x))/nom;
                if (temp_step < step) {
                    step = temp_step;
                }
            }
        }
     }

     /* copy and return new matrix */
     matrix * matrix_copy(matrix* source) {
        //TODO check
        matrix* m = create_matrix(source->rows,source->columns);
        memcpy(m->start,source->start,source->size);

     }

     /* calculate the derivative for subproblem, and solve for = 0 */
     bool derive_and_solve(matrix* G, matrix* g, matrix* p) {
        matrix* Qd = create_matrix(G->rows,G->columns);
        matrix* Gt = create_matrix(G->rows,G->columns);
        transpose_matrix(G,Gt);

        add_matrices(G, Gt, Qd);

        matrix* gm = matrix_copy(g);
        multiply_matrix_with_scalar(-1,gm);

        //TODO
        /* solves linear system Ax = b
         * linear_solve(A,b,x) 
         * puts answer into p */
        linear_solve(Gt,gm,p);


        return true;

     }
