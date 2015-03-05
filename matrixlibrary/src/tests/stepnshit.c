

#include<stdio.h>
#include<matLib.h>
/*

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

    typedef struct work_set work_set;

    bool work_set_create(work_set* ws, int ws_max) {
        //TODO validate indata
        ws->data = malloc(ws_max*sizeof(int));
        ws->count = 0;
        return true;
    }

    bool work_set_append(,work_set* ws, int val) {
        //TODO validate indata
        ws->data[ws->count] = val;
        ws->count++;
        return true;
    }

    bool work_set_remove(work_set* ws, int val) {
        //TODO validate indata
        //maybe want to keep order of conditions, but for now: mess up order on remove!
        for (int i = 0; i < ws->count; i++) {
            if (ws->data[i] == val) {
                if (i == count-1)
                {
                    ws->count--;
                    return true;
                }
                else {
                    ws->data[i] = ws->data[count-1];
                    ws->count--;
                    return true;
                }
            }
        }
        return false;
    }




     /* calculates step for active set-method */
     value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws) {
        matrix* ai, ati;
        value bi, nom, temp_step, step = 1;

        //TODO check
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

     /* solves quadratic convex problem in the form min(z) (1/2) * z^T*Q*z + q*z 
      * s.t. Az >= b,  or  Az >= b + s


     */

     matrix* quadopt_solver(matrix* z0, matrix* Q, matrix* q) {

        /* create variables */
        matrix * A,B; //osv
        work_set * active_set, sub_set;



        /* load matrixes from file(s) */
        A = load_matrix("matrices.qopt");


        /* calculate matrix transposes, derivatives. set variables */
        At = transpose_matrix(A);


        //TODO check if all matrix dimensions are correct




        /* set initial working set */

        //TODO solve linear system A*z0 = b,  or  A*z0 = b + s;

        work_set_create(active_set, A->rows);
        for (int i = 1; i <= A->rows; i++) {
            int ans = 0;
            for (int j = 1; j <= A->columns; j++) {
                ans += get_value(i,j,A)*get_value(j,1,z0);
            }

            if (ans == get_value(i,1,b)) { //+get_value(i,0,s)
                work_set_append(active_set,i);
            }
        }

        if (active_set->count == 0)
        {}


        while (!is_positive_matrix(lagrange) && step < step_treshold) {


        }




     }
