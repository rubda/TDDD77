#include <stdio.h>
#include <matLib.h>
#include "solver.h"
#include "find_lagrange.h"




qp_problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, matrix* F, matrix* g, matrix* z0) {

  qp_problem* prob = malloc(sizeof(qp_problem));

  /* objective function */
  prob->Q = Q;
  matrix* Qn = create_matrix(Q->rows,Q->columns);
  get_inverse(Q,Qn);
  prob->Qn = Qn;

  prob->q = q;

  /* constraints */
  prob->E = E;
  prob->h = h;
  prob->F = F;
  prob->g = g;

  if (E == NULL) {
    prob->equality_count = 0;
  }
  else {
    prob->equality_count = E->rows;
  }
  if (F == NULL) {
    prob->inequality_count = 0;
  }
  else {
    prob->inequality_count = F->rows;
  }

  prob->constraints_count = prob->equality_count + prob->inequality_count;

  prob->A = create_matrix(prob->constraints_count,q->rows);
  prob->b = create_matrix(prob->constraints_count,1);
  int r;

  /* insert equality constraints */
  matrix* temp_row;
  for (r = 1; r <= prob->equality_count; r++) {
    temp_row = get_row_vector_with_return(r,E);
    insert_row_vector(r,temp_row,prob->A);
    insert_value_without_check(get_value_without_check(r,1,h),r,1,prob->b);
    free_matrix(temp_row);
  }

  /* insert inequality constraints */
  for (r = 1; r <= prob->inequality_count; r++) {
    temp_row = get_row_vector_with_return(r,F);
    insert_row_vector(r+prob->equality_count,temp_row,prob->A);
    insert_value_without_check(get_value_without_check(r,1,h),r+prob->equality_count,1,prob->b);
    free_matrix(temp_row);
  }

  /* points and vectors */  
  if (z0 == NULL) {
    prob->has_start_point = false;
    prob->z0 = create_matrix(q->rows,1);
  }
  else {
    prob->has_start_point = true;
    prob->z0 = z0;
  }  
  prob->z = create_matrix(q->rows,1);;
  prob->has_solution = false;
  prob->solution = create_matrix(q->rows,1);

  prob->lagrange = create_matrix(prob->A->rows,1);
  prob->p = create_matrix(q->rows,1);

  /* work set */
  prob->active_set = work_set_create(prob->A->rows);

  return prob;
}

void print_qp_problem(qp_problem* prob) {


}

void solve_subproblem(qp_problem* prob) {

}

/* calculates step for active set-method *//*
value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws) {
  matrix* ai, *ati;
  ati = create_matrix(A->columns, 1);
  ai = create_matrix(1, A->columns);
  value bi, nom, temp_step, step = 1;

  for (int i = 1; i <= A->rows; i++) {
    if (work_set_contains(ws,i)) {
      continue;
    }
    get_row_vector(i, A, ai);
    transpose_matrix(ai, ati);
    nom = dot_product(ati,p);

    if (nom < 0) {
      bi = get_value(i,1,B);
      temp_step = (bi - dot_product(ati,x))/nom;
      if (temp_step < step) {
        step = temp_step;
      }
    }
  }
  free_matrix(ai);
  free_matrix(ati);
  return step;
}

/* checks if the lagrange multipliers in the active set is positive *//*
bool is_positive_lagrange(matrix* l, work_set* ws) {
  for (int i = 0; i < ws->count; i++) {
    if (get_value_without_check(ws->data[i],1,l) < 0) {
      return false;
    }
  }
  return true;
}*/

bool is_positive_lagrange1(qp_problem* prob) {
  return true;
}

/*
bool fill_active_set(matrix* z, matrix* A, matrix* b, work_set* ws) {
  /* clear */
  /*work_set_clear(ws);

  /* fill */
  /*for (int i = 1; i <= A->rows; i++) {
    int ans = 0;
    for (int j = 1; j <= A->columns; j++) {
      ans += get_value(i,j,A)*get_value(j,1,z); 
      //TODO add check and get_value_without_check and return false
    }

    if (ans == get_value(i,1,b)) { //+get_value(i,0,s)
      work_set_append(ws,i);
    }
  }

  return true;
}*/

void remove_constraint(qp_problem* prob) {

}

bool fill_active_set1(qp_problem* prob) {
  return true;
}

void take_step(qp_problem* prob) {

}

matrix* quadopt_solver1(qp_problem* prob) {

  /* calculate starting point if needed */
  if (!prob->has_start_point) {
    /*
    calculate_starting_point(prob);
    */
  }

  fill_active_set1(prob);

  while (true) {
    solve_subproblem(prob);

    if (is_zero_matrix(prob->p)) {
      if (prob->active_set->count == 0) {
        break;
      }
      if (is_positive_lagrange1(prob)) {
        break;
      }
      else {
        remove_constraint(prob);
      }
    }
    else {

      take_step(prob);
      /* set active set */
      fill_active_set1(prob);
    }
  }

  matrix_copy_data(prob->z,prob->solution);
  return prob->solution;
}


/* solves quadratic convex problem in the form min(z) (1/2) * z^T*G*z + d*z 
 * s.t. Az >= b
 */
 /*
matrix* quadopt_solver(qp_problem* prob, matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy) {



  /* create variables */
  /*matrix* p = matrix_copy(z0); //unessecary init of values, only has to be the same dims
  matrix* gk = matrix_copy(d);
  matrix* z_last = matrix_copy(z0);
  matrix * z = matrix_copy(z0);
  matrix* lagrange = create_matrix(A->rows,1); //osv

  work_set* active_set = work_set_create(A->rows);

  value step;


  /* calculate matrix transposes, derivatives. set variables */
  /*matrix* G_derivate = matrix_copy(G);
  multiply_matrix_with_scalar(2,G_derivate);
  int counter = 0;

  //******************** solve the problem ********************/

  /* set active set */
  /*fill_active_set(z,  A, b, active_set);



  do {
    printf("\n\n\n------------------------------------------------------\n");
    printf("Iteration: %d\n",counter);

    print_matrix(z);
    print_matrix(A);
    print_matrix(b);

    /* set active set */
    //fill_active_set(z,  A, b, active_set);

    /*printf("Before sub-problem: ");
    work_set_print(active_set);


    /* calculate gk */
    /*multiply_matrices(G,z,gk);
    add_matrices(gk,d,gk);

    /******************** solve sub-problem ********************/

    /*matrix* temp_A = matrix_copy(A);

    /* get solution for sub problem */		
    //get_p(temp_A, G, gk, d, z, p, lagrange, active_set);
    /*solve_subproblem(temp_A, G, gk, d, z, p, lagrange, active_set);

    printf("After sub-problem: ");
    work_set_print(active_set);

    printf("vector p = \n");
    print_matrix(p);


    /* check second derivative if minimum */
    //is_positive_diagonal_matrix(G_derivate);
    //TODO if not minimum?


    /* calculate step */
    /*step = calculate_step(b, A, z, p, active_set);

    printf("step: %f\n",step);

    /* take step */
    /*matrix_copy_data(z,z_last); //TODO implement this function
    multiply_matrix_with_scalar(step,p);
    add_matrices(z_last,p,z);

    if (is_zero_matrix(p)) {
      if (active_set->count > 0) {
        find_lagrange(gk, A, d, z, active_set, lagrange);
      }
      else {
        break;
      }
    }
    else {
      if (active_set->count > 0) {
        if (is_positive_lagrange(lagrange,active_set)) {
            break;
        }
      }
      /* set active set */
      /*fill_active_set(z,  A, b, active_set);
    }

    /* */



    /*counter++;
  } while (true); //!(is_positive_lagrange(lagrange, active_set) && is_zero_matrix(p)));  //TODO  add condition: if step <= accuracy then stop
  //implement is_positive_langrange and is_zero_matrix

  return z;


}
*/