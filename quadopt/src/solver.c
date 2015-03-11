#include <stdio.h>
#include <matLib.h>
#include "solver.h"
#include "find_lagrange.h"

/* calculates step for active set-method */
value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws) {
  matrix* ai, *ati;
  ati = create_matrix(A->columns, 1);
  ai = create_matrix(1, A->columns);
  value bi, nom, temp_step, step = 1;



  //TODO check
  for (int i = 0; i < A->rows; i++) {
  	if (work_set_contains(ws,i)) {
  		continue;
  	}
		get_row_vector(i, A, ai); //TODO free this later
		transpose_matrix(ai, ati);
		nom = vector_product(ati,p);
		if (nom < 0) {
		  bi = get_value(i,1,B);
		  temp_step = (bi - vector_product(ati,x))/nom;
		  if (temp_step < step) {
				step = temp_step;
		  }
		}
  }
  return step;
}

/* checks if the lagrange multipliers in the active set is positive */
bool is_positive_lagrange(matrix* l, work_set* ws) {
  for (int i = 0; i < ws->count; i++) {
	if (get_value_without_check(ws->data[i],1,l) < 0) {
	  return false;
	}
  }
  return true;
}

bool solve_active_conditions(matrix* Ain, matrix* x, work_set* set) {

	/* check if the system is solveable */
	if (set->count < Ain->columns) {
		return false;
	}

  matrix* A = create_matrix(set->count, Ain->columns);
  matrix* row = create_matrix(1, Ain->columns);

  matrix* b = create_matrix(set->count, 1);

  /* build matrices */
  for (int i = 0; i < set->count; i++) {
	get_row_vector(set->data[i], Ain, row);
	insert_row_vector(i+1, row, A);
	insert_value_without_check(0, i+1, 1, b);
  }



  solve_linear(A, x, b);


  /* free used matrices */
  free_matrix(A);
  free_matrix(b);

  //TODO check if linear system is solveable or x = 0 afterwards
  bool dependancy = false;
  if (!dependancy) {
	return true;
  }
  else {
	return false;
  }

}

	 


/* solves quadratic convex problem in the form min(z) (1/2) * z^T*G*z + d*z 
 * s.t. Az >= b
 */
matrix* quadopt_solver(matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy) {

  /* create variables */
  matrix* p = matrix_copy(z0); //unessecary init of values, only has to be the same dims
  matrix* gk = matrix_copy(d);
  matrix* z_last = matrix_copy(z0);
  matrix * z = matrix_copy(z0);
  matrix* temp;
  matrix* lagrange = create_matrix(A->rows,1); //osv

  work_set* active_set = work_set_create(A->rows);;

  value step;


  /* calculate matrix transposes, derivatives. set variables */

  //At = transpose_matrix(A);
  matrix* G_derivate = matrix_copy(G);
  multiply_matrix_with_scalar(2,G_derivate);
  int counter = 0;

  //******************** solve the problem ********************/
  do {
  	printf("\n\n\n------------------------------------------------------\n");
  	printf("Iteration: %d\n",counter);

		/* set active set */
		for (int i = 1; i <= A->rows; i++) {
		  int ans = 0;
		  for (int j = 1; j <= A->columns; j++) {
		ans += get_value(i,j,A)*get_value(j,1,z); 
		//TODO add check and get_value_without_check
		  }

		  if (ans == get_value(i,1,b)) { //+get_value(i,0,s)
		work_set_append(active_set,i);
		  }
		}

		printf("Before sub-problem: ");
		work_set_print(active_set);


		/* calculate gk */
		multiply_matrices(G,z,gk);
		add_matrices(gk,d,gk);

		matrix* neg_gk = matrix_copy(gk);
		multiply_matrix_with_scalar(-1,neg_gk);



		/******************** solve sub-problem ********************/

		matrix* temp_A = matrix_copy(A);
		matrix* temp_b = matrix_copy(b);

		

		/* solve system until we reach a linear dependancy or not zero vector */
		while (solve_active_conditions(temp_A, p, active_set)) {
	  	/* calculate lagrange multipliers and remove possible condition from active set */
	  	find_lagrange(gk, A, d, z, active_set, lagrange);
		}

		printf("After sub-problem: ");
		work_set_print(active_set);
		/* solve linear system for 1st derivative*/

		solve_linear(G, p, neg_gk);

		printf("vector p = \n");
		print_matrix(p);
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

		counter++;
  } while (!is_positive_lagrange(lagrange, active_set) && !is_zero_matrix(p));  //TODO  add condition: if step <= accuracy then stop
  //implement is_positive_langrange and is_zero_matrix

  return z;


}
