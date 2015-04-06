#include <stdio.h>
#include "matLib.h"
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
  prob->gk = create_matrix(q->rows,1);

  /* work set */
  prob->active_set = work_set_create(prob->A->rows);

  return prob;
}

void print_qp_problem(qp_problem* prob) {


}


void solve_subproblem(qp_problem* prob) {

  //work_set_copy(prob->active_set,prob->subset);
  /* calculate variables necessary for sub problem */

 /* gk */
  matrix* tmp = create_matrix(prob->q->rows,1);
  matrix* gk = create_matrix(prob->q->rows,1);
  multiply_matrices(prob->Q,prob->z,tmp);
  add_matrices(tmp,prob->q,gk);
  free_matrix(tmp);



  if (prob->active_set->count == 0) {
    /* solve derivative and get vector pointing towards the global minimum */
    value sum, d_val;
    int r, c;

    for (c = 1; c <= prob->Q->columns; c++) {
      sum = 0;
      for (r = 1; r <= prob->Q->rows; r++) {
        sum += get_value_without_check(r,c,prob->Q);
      }
      d_val = get_value_without_check(c,1,gk);
      insert_value_without_check((-d_val)/sum,c,1,prob->p);
    }
    return;
  }

  /* solve system as long as you get the the zero vector */
  matrix* A = get_active_conditions(prob);
  //matrix* zero = get_zero_matrix(prob->p->rows, prob->p->columns);

  bool success;
  do{
    success = gauss_jordan(A);

    if(!success){
      printf("Could not solve conditions to subproblem!\n");
    }else{
      printf("Solution from conditions in subproblem:\n");
      print_matrix(prob->p);

      /* Remove condition */
      //find_lagrange(gk, A, d, z, ws, lagrange);
      
      /* Resize A matrix */
      free_matrix(A);
      A = get_active_conditions(prob);
    }
  }while(success);

  /* use range space to get p */

  matrix* At = transpose_matrix_with_return(A);  

  matrix* AQ = create_matrix(A->rows,prob->Qn->columns);
  multiply_matrices(A,prob->Qn,AQ);  

  matrix* AQAt = create_matrix(AQ->rows,At->columns);
  multiply_matrices(AQ,At,AQAt);  

  matrix* AQg = create_matrix(AQ->rows,gk->columns);
  multiply_matrices(AQ,gk,AQg);  

  matrix* Az = create_matrix(A->rows,prob->z->columns);
  multiply_matrices(A,prob->z,Az);  

  matrix* h1 = create_matrix(AQg->rows,AQg->columns); //matrix_copy(AQg);
  subtract_matrices(AQg, Az, h1);  

  matrix* lambda = create_matrix(AQg->rows,AQg->columns); //matrix_copy(h1);
  solve_linear(AQAt,lambda,h1);  

  matrix* ht = create_matrix(prob->p->rows,lambda->columns);
  matrix* h2 = create_matrix(ht->rows,ht->columns);
  multiply_matrices(At,lambda,ht);
  subtract_matrices(ht,gk,h2);

  solve_linear(prob->Q,prob->p,h2);


  #ifdef DEBUG

  printf("\n----- A -----\n");
  print_matrix(A);

  printf("\n----- At -----\n");
  print_matrix(At);

  printf("\n----- Q_inv -----\n");
  print_matrix(prob->Qn);

  printf("\n----- AQ -----\n");
  print_matrix(AQ);
  
  printf("\n----- AQAt -----\n");
  print_matrix(AQAt);

  printf("\n----- AQg -----\n");
  print_matrix(AQg);

  printf("\n----- Az -----\n");
  print_matrix(Az);

  printf("\n----- h1 -----\n");
  print_matrix(h1);

  printf("\n----- lambda -----\n");
  print_matrix(lambda);

  printf("\n----- h2 -----\n");
  print_matrix(h2);

  print_matrix(p);

  #endif

  /*TODO FREE everything*/
  //free_matrix(zero);
  free_matrix(A);
}

matrix* get_active_conditions(qp_problem* prob){
  matrix* A = create_matrix(prob->active_set->count, prob->A->columns);
  
  bool success = false;
  for(int i = 0; i < prob->active_set->count; i++){
    matrix* temp_row = get_row_vector_with_return(prob->active_set->data[i], prob->A);
    success = insert_row_vector(i + 1, temp_row, A);
    free_matrix(temp_row);
  }

  if(success){
    return A;
  }else{
    free_matrix(A);
    return NULL;
  }
}

matrix* get_zero_matrix(int rows, int columns){
  matrix* zero = create_matrix(rows, columns);
  free(zero->start);
  zero->start = calloc(rows * columns, sizeof(value));

  if(zero->start == NULL){
    return NULL;
  }

  return zero;
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

bool remove_constraint(qp_problem* prob) {
  
  matrix* ait;
  matrix* ai;
  matrix* LA = create_matrix(prob->p->rows,prob->active_set->count);
  matrix* lagrange = create_matrix(prob->active_set->count,1);

  for (int i = 1; i <= prob->active_set->count; i++) {
    ai = get_row_vector_with_return(prob->active_set->data[i-1],prob->A);
    ait = transpose_matrix_with_return(ai);
    insert_column_vector(i, ait, LA);
    free(ai);
    free(ait);
  }

  if (solve_linear(LA,lagrange,prob->gk)) {    

  }
  else {
    least_square(LA,lagrange,prob->gk);
  }

  /* find most negative and remove (if not equality constraint) */
  int small;
  value tmp;
  value val = 0;
  for (int i = 1; i <= lagrange->rows; i++) {
    if (prob->active_set->data[i-1] <= prob->equality_count) {
      continue;
    }
    tmp = get_value_without_check(i,1,lagrange);
    if (tmp < val) {
      small = prob->active_set->data[i-1];
      val = tmp;
    }
  }
  if (val < 0) {
    /* remove */
    work_set_remove(prob->active_set,small);
    return true;
  }

  /* could not remove any constraints */
  return false;

}

bool fill_active_set(qp_problem* prob) {
  /* clear */
  work_set_clear(prob->active_set);

  /* fill */
  for (int i = 1; i <= prob->A->rows; i++) {
    if (i <= prob->equality_count) {
      work_set_append(prob->active_set,i);
      continue;
    }
    int ans = 0;
    for (int j = 1; j <= prob->A->columns; j++) {
      ans += get_value(i,j,prob->A)*get_value(j,1,prob->z); 
      //TODO add check and get_value_without_check and return false
    }

    if (ans == get_value(i,1,prob->b)) { //+get_value(i,0,s)
      work_set_append(prob->active_set,i);
    }
  }
  return true;
}

void take_step(qp_problem* prob) {
  matrix* ai, *ati;
  ati = create_matrix(prob->A->columns, 1);
  ai = create_matrix(1, prob->A->columns);
  matrix* z_old = matrix_copy(prob->z);
  value bi, nom, temp_step, step = 1;

  /* only go through the inequality constraints */
  for (int i = prob->equality_count+1; i <= prob->A->rows; i++) {
    if (work_set_contains(prob->active_set,i)) {
      continue;
    }
    get_row_vector(i, prob->A, ai);
    transpose_matrix(ai, ati);
    nom = dot_product(ati,prob->p);

    if (nom < 0) {
      bi = get_value(i,1,prob->b);
      temp_step = (bi - dot_product(ati,prob->z))/nom;
      if (temp_step < step) {
        step = temp_step;
      }
    }
  }

  multiply_matrix_with_scalar(step,prob->p);
  add_matrices(z_old,prob->p,prob->z);

  /* free matrices */
  free_matrix(ai);
  free_matrix(ati);
  free_matrix(z_old);
}

matrix* quadopt_solver(qp_problem* prob) {

  /* calculate starting point if needed */
  if (!prob->has_start_point) {
    /*
    calculate_starting_point(prob);
    */
  }

  fill_active_set(prob);

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
      fill_active_set(prob);
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