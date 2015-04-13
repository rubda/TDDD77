#include <stdio.h>
#include <matLib.h>
#include "solver.h"
#include "find_lagrange.h"
#include <math.h>

bool remove_constraint(qp_problem* prob);

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
    insert_value_without_check(get_value_without_check(r,1,g),r+prob->equality_count,1,prob->b);
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

  printf("\n********** Quadratic Problem **********\n\n");

  printf("Objective function: \n");
  printf("Q = \n");
  print_matrix(prob->Q);
  printf("q = \n");
  print_matrix(prob->q);

  printf("Equality constraints: \n");
  if (prob->E != NULL) {
    printf("E = \n");
    print_matrix(prob->E);
    printf("h = \n");
    print_matrix(prob->h);
  }
  else {
    printf("-\n\n");
  }

  printf("Inequality constraints: \n");
  if (prob->F != NULL) {
    printf("F = \n");
    print_matrix(prob->F);
    printf("g = \n");
    print_matrix(prob->g);
  }
  else {
    printf("-\n\n");
  }

  printf("Properties: \n");
  printf("Number of variables = %d\n",prob->z->rows);
  printf("Number of constraints = %d\n",prob->constraints_count);
  printf("Accuracy = %f\n\n",1-prob->accuracy);

  printf("Starting point: \n");
  if (prob->has_start_point) {    
    printf("z = \n");
    print_matrix(prob->z0);
  }
  else {
    printf("Not set or calculated yet.\n\n");
  }

  printf("Solution: \n");
  if (prob->has_solution) {
    printf("z = \n");
    print_matrix(prob->solution);
  }
  else {
    printf("Not calculated yet.\n\n");
  }

  printf("***************************************\n");

}


void free_qp_problem(qp_problem* prob) {
  free_matrix(prob->Q);
  free_matrix(prob->Qn);

  free_matrix(prob->q);

  free_matrix(prob->E);
  free_matrix(prob->h);

  free_matrix(prob->F);
  free_matrix(prob->g);

  free_matrix(prob->A);
  free_matrix(prob->b);

  free_matrix(prob->z0);

  free_matrix(prob->z);

  free_matrix(prob->solution);

  free_matrix(prob->p);
  free_matrix(prob->gk);
  free_matrix(prob->lagrange);

  work_set_free(prob->active_set);

  free(prob);
}


void solve_subproblem(qp_problem* prob) {

 /* gk */
  matrix* tmp = create_matrix(prob->q->rows,1);
  multiply_matrices(prob->Q,prob->z,tmp);
  add_matrices(tmp,prob->q,prob->gk);
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
      d_val = get_value_without_check(c,1,prob->gk);
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

    if(success){
      /* Remove condition */
      remove_constraint(prob);
      
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

  matrix* AQg = create_matrix(AQ->rows,prob->gk->columns);
  multiply_matrices(AQ,prob->gk,AQg);  

  matrix* Az = create_matrix(A->rows,prob->z->columns);
  multiply_matrices(A,prob->z,Az);  

  matrix* h1 = create_matrix(AQg->rows,AQg->columns); //matrix_copy(AQg);
  subtract_matrices(AQg, Az, h1);  

  matrix* lambda = create_matrix(AQg->rows,AQg->columns); //matrix_copy(h1);
  solve_linear(AQAt,lambda,h1);  

  matrix* ht = create_matrix(prob->p->rows,lambda->columns);
  matrix* h2 = create_matrix(ht->rows,ht->columns);
  multiply_matrices(At,lambda,ht);
  subtract_matrices(ht,prob->gk,h2);

  solve_linear(prob->Q,prob->p,h2);



  #ifdef DEBUG

  printf("\n----- A -----\n");
  print_matrix(A);

  printf("\n----- At -----\n");
  print_matrix(At);

  printf("\n----- Q -----\n");
  print_matrix(prob->Q);

  printf("\n----- Q_inv -----\n");
  print_matrix(prob->Qn);

  printf("\n----- gk -----\n");
  print_matrix(prob->gk);

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


  printf("\n------ p -----\n");
  print_matrix(prob->p);
  #endif


  matrix* Qp = create_matrix(prob->gk->rows, prob->gk->columns);
  multiply_matrices(prob->Q, prob->p, Qp);

  if(compare_matrices(Qp, prob->gk)){
    for(int i = 1; i <= prob->p->rows; i++){
      insert_value_without_check(0, i, 1, prob->p);
    }
  }


  /*TODO FREE everything*/
  //free_matrix(zero);
  free_matrix(A);
  free_matrix(At);
  free_matrix(AQ);
  free_matrix(AQAt);
  free_matrix(AQg);
  free_matrix(Az);
  free_matrix(h1);
  free_matrix(lambda);
  free_matrix(ht);
  free_matrix(h2);
  free_matrix(Qp);
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

bool is_positive_lagrange(qp_problem* prob) {
  
  matrix* ait;
  matrix* ai;
  matrix* LA = create_matrix(prob->p->rows,prob->active_set->count);
  matrix* lagrange = create_matrix(prob->active_set->count,1);

  for (int i = 1; i <= prob->active_set->count; i++) {
    ai = get_row_vector_with_return(prob->active_set->data[i-1],prob->A);
    ait = transpose_matrix_with_return(ai);
    insert_column_vector(i, ait, LA);
    free_matrix(ai);
    free_matrix(ait);
  }

  if (solve_linear(LA,lagrange,prob->gk)) {    

  }
  else {
    least_square(LA,lagrange,prob->gk);
  }
  for (int i = 1; i <= lagrange->rows; i++) {
    if (prob->active_set->data[i-1] <= prob->equality_count) { //Is this correct?
      continue;
    }
    if (get_value_without_check(i,1,lagrange) < 0) {
      free_matrix(LA);
      free_matrix(lagrange);
      return false;
    }
  }
  free_matrix(LA);
  free_matrix(lagrange);
  return true;
}

bool remove_constraint(qp_problem* prob) {
  
  matrix* ait;
  matrix* ai;
  matrix* LA = create_matrix(prob->p->rows,prob->active_set->count);
  matrix* lagrange = create_matrix(prob->active_set->count,1);

  for (int i = 1; i <= prob->active_set->count; i++) {
    ai = get_row_vector_with_return(prob->active_set->data[i-1],prob->A);
    ait = transpose_matrix_with_return(ai);
    insert_column_vector(i, ait, LA);
    free_matrix(ai);
    free_matrix(ait);
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
    free_matrix(LA);
    free_matrix(lagrange);
    return true;
  }

  /* could not remove any constraints */
  free_matrix(LA);
  free_matrix(lagrange);
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
    value ans = 0;
    for (int j = 1; j <= prob->A->columns; j++) {
      ans += get_value(i,j,prob->A)*get_value(j,1,prob->z); 
      //TODO add check and get_value_without_check and return false
    }
    if (compare_elements(ans, get_value_without_check(i,1,prob->b))) { //+get_value(i,0,s)
      work_set_append(prob->active_set,i);
    }
  }

  return true;
}

bool take_step(qp_problem* prob) {
  matrix* ai, *ati;
  ati = create_matrix(prob->A->columns, 1);
  ai = create_matrix(1, prob->A->columns);
  matrix* z_old = matrix_copy(prob->z);
  value bi, nom, temp_step, step = 1;

  /* only go through the inequality constraints */
  for (int i = 1; i <= prob->A->rows; i++) { //prob->equality_count
    if (work_set_contains(prob->active_set,i)) {
      continue;
    }
    get_row_vector(i, prob->A, ai);
    transpose_matrix(ai, ati);
    nom = dot_product(ati,prob->p);
    if (!compare_elements(nom,0)) {
      bi = get_value(i,1,prob->b);
      temp_step = (bi - dot_product(ati,prob->z))/nom;
      if (temp_step >= 0 && temp_step < step) {
        step = temp_step;
      }
    }
  }

  /* free matrices and return */
  if (!compare_elements(step,0)) {
    multiply_matrix_with_scalar(step,prob->p);    
    add_matrices(z_old,prob->p,prob->z);

    free_matrix(ai);
    free_matrix(ati);
    free_matrix(z_old);
    return true;
  }
  
  free_matrix(ai);
  free_matrix(ati);
  free_matrix(z_old);
  return false;
}

matrix* quadopt_solver(qp_problem* prob) {

  /* calculate starting point if needed */
  if (!prob->has_start_point) {
    /*
    calculate_starting_point(prob);
    */
  }
  else {
    matrix_copy_data(prob->z0, prob->z);
  }

  fill_active_set(prob);

  while (true) {

    solve_subproblem(prob);

    if (is_zero_matrix(prob->p)) {
      if (prob->active_set->count == 0) {
        break;
      }
      if (is_positive_lagrange(prob)) {
        break;
      }
      else {
        remove_constraint(prob);
      }
    }
    else {

      /* could not move */
      if (!take_step(prob)) {
        break;
      }
      /* set active set */
      fill_active_set(prob);
    }
  }

  matrix_copy_data(prob->z,prob->solution);
  prob->has_solution = true;
  return prob->solution;
}

