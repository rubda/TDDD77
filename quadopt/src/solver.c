

#include <stdio.h>
#include <matLib.h>
#include "solver.h"


     /* calculates step for active set-method */
     value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws) {
        matrix* ai, *ati;
        value bi, nom, temp_step, step = 1;

        //TODO check
        for (int i = 0; i < ws->count; i++) {
            get_row_vector(ws->data[i], A, ati); //TODO free this later
            nom = vector_product(ati,p);
            if (nom < 0) {
                bi = get_value(ws->data[i],1,B);
                temp_step = (bi - vector_product(ati,x))/nom;
                if (temp_step < step) {
                    step = temp_step;
                }
            }
        }
        return step;
     }

     bool is_positive_lagrange(matrix* l, work_set* ws) {
        for (int i = 0; i < ws->count; i++) {
            if (get_value_without_check(ws->data[i],1,l) < 0) {
                return false;
            }
        }
        return true;
     }

     


     /* solves quadratic convex problem in the form min(z) (1/2) * z^T*G*z + d*z 
      * s.t. Az >= b
    

     */
     //TODO define where matrices comes from
    //      s.t. Az >= b + s
     matrix* quadopt_solver(matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy) {

        /* create variables */
        matrix* p = matrix_copy(z0); //unessecary init of values, only has to be the same dims
        matrix* gk = matrix_copy(d);
        //matrix* z = matrix_copy(z0);
        matrix* z_last = matrix_copy(z0);
        matrix* lagrange = create_matrix(A->rows,1); //osv

        work_set* active_set;

        value step;

        /* load matrixes from file(s) */
        //A = load_matrix("matrices.qopt");


        /* calculate matrix transposes, derivatives. set variables */
        //At = transpose_matrix(A);

        matrix* G_derivate = matrix_copy(G);
        multiply_matrix_with_scalar(2,G_derivate);


        //TODO check if all matrix dimensions are correct




        /* set initial working set */

        

        active_set = work_set_create(A->rows);

        matrix * z = matrix_copy(z0);
        matrix* temp;


        //******************** solve the problem ********************/
        do {
            /* set active set */
            for (int i = 1; i <= A->rows; i++) {
                int ans = 0;
                for (int j = 1; j <= A->columns; j++) {
                    ans += get_value(i,j,A)*get_value(j,1,z); //TODO add check and get_value_without_check
                }

                if (ans == get_value(i,1,b)) { //+get_value(i,0,s)
                    work_set_append(active_set,i);
                }
            }


            /* calculate gk */
            multiply_matrices(G,z,gk);
            add_matrices(gk,d,gk);

            matrix* neg_gk = matrix_copy(gk);
            multiply_matrix_with_scalar(-1,neg_gk);

            /******************** solve sub-problem ********************/

            /* calculate lagrange multipliers */
            find_lagrange(neg_gk, A, d, z, active_set, lagrange);


            /*calculate_lagrange(A, lagrange, active_set, gk); //TODO implement this function


            /* remove the condition that has the most negative lagrange multiplicator */
            /*int temp = get_value_without_check(1, 1, lagrange);
            int smallest = 1;
            for (int i = 0; i <= active_set->count; i++) {
                if (get_value_without_check(active_set->data[i], 1, lagrange) < temp) {
                    smallest = active_set->data[i];
                }
            }
            work_set_remove(active_set, smallest);*/


            /* solve linear system for 1st derivative*/
            solve_linear(G_derivate, p, neg_gk);


            /* check second derivative if minimum */
            //is_positive_diagonal_matrix(G_derivate);
            //TODO if not minimum?


            /* calculate step */
            step = calculate_step(b, A, z, p, active_set);

            /* take step */
            matrix_copy_data(z,z_last); //TODO implement this function
            multiply_matrix_with_scalar(step,p);
            add_matrices(z_last,p,z);

            /* */

        } while (!is_positive_lagrange(lagrange, active_set) && !is_zero_vector(p));  //TODO  add condition: if step <= accuracy then stop
                                                                        //implement is_positive_langrange and is_zero_matrix

        return z;


     }
