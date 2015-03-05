

#include<stdio.h>
#include<matLib.h>



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
        return m;
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

     /* solves quadratic convex problem in the form min(z) (1/2) * z^T*G*z + G*z 
      * s.t. 1) Az >= b,  or  2) Az >= b + s
      * fokus on (1) for now


     */

     matrix* quadopt_solver(matrix* z0, matrix* G, matrix* g {

        /* create variables */
        matrix * A;
        matrix* b;
        matrix* lagrange; 
        matrix* p;
        matrix* z;
        //osv

        work_set* active_set;
        work_set* sub_set;

        value step = 0;




        /* load matrixes from file(s) */
        A = load_matrix("matrices.qopt");


        /* calculate matrix transposes, derivatives. set variables */
        At = transpose_matrix(A);


        //TODO check if all matrix dimensions are correct


        /* algoritm */

        z = z0;

        while(!is_zero_vector(p) && !is_positive_lagrange(lagrange, active_set))
        {
            





            /* set working set */
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

            matrix* Ak = create_matrix(active_set->count,A->columns);
            matrix* bk = create_matrix(active_set->count,1);
            for (int i = 0; i < active_set->count; i++) {
                insert_row(active_set->data[i], A, i+1, Ak); //insert row i from A to row j in Ak, needs to implemented
                insert_row(active_set->data[i], b, i+1, bk);
            }


            /* solve subproblem
             * (1/2)*p^T*G*p + gk*p 
             * s.t. Ak*p = 0
             * p = 0 is always an answer??
             */

             /* compute lagrange multiplikatros 
              * solve Ak*l = gk
              *
              */



            
            /* calculate new vectors
             * z_last = z;
             * z += p*step; 
             */

        }




     }
