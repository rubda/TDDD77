/*
  Author: Ruben Das
  Email: rubda680@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to retrieve values from the matrices given.
*/

#include <problem.h>


void fill_constraint_matrices(problem* prob) {
  prob->constraints_count = prob->equality_count + prob->inequality_count;


  if (prob->constraints_count != 0) {
    /* All constrains lhs */
    prob->A = create_matrix(prob->constraints_count, prob->variable_count);
    /* All constrains rhs */
    prob->b = create_matrix(prob->constraints_count, 1);

    if (prob->is_sparse) {
      /* create array with sparse rows */
      prob->sparse_A = malloc(prob->constraints_count*sizeof(sparse_matrix*));
    }

  }

  /* Insert equality constraints */
  matrix* temp_row;
  sparse_matrix* s_temp;
  int r;
  for (r = 1; r <= prob->equality_count; r++){
    temp_row = get_row_vector_with_return(r, prob->E);
    insert_row_vector(r, temp_row, prob->A);
    insert_value_without_check(get_value_without_check(r, 1, prob->h), r, 1, prob->b);

    if (prob->is_sparse) {
      s_temp = create_sparse_matrix(temp_row, -1);
      prob->sparse_A[r-1] = s_temp;
    }

    free_matrix(temp_row);
  }

  /* Insert inequality constraints */
  for (r = 1; r <= prob->inequality_count; r++){
    temp_row = get_row_vector_with_return(r, prob->F);
    insert_row_vector(r+prob->equality_count, temp_row, prob->A);
    insert_value_without_check(get_value_without_check(r, 1, prob->g), r+prob->equality_count, 1, prob->b);

    if (prob->is_sparse) {
      s_temp = create_sparse_matrix(temp_row, -1);
      prob->sparse_A[r-1+prob->equality_count] = s_temp;
    }

    free_matrix(temp_row);
  }
}


/* Allocates the problem and sets all necessary variables */
problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, matrix* F, matrix* g,	matrix* z0, int max_iter, int max_micro_sec){

  problem* prob = (problem*)malloc(sizeof(problem));

  /* Objective function */
  prob->Q = Q;
  matrix* Q_inv = create_matrix(Q->rows, Q->columns);
  get_inverse(Q, Q_inv);
  prob->Q_inv = Q_inv;
 

  prob->q = q;

  prob->variable_count = Q->rows;

  /* Constraints */

  /* Equality constraints */
  prob->E = E;

  if (E == NULL){
    prob->equality_count = 0;
  }else{
    prob->equality_count = E->rows;
  }
  /* Right hand side for E */
  prob->h = h;


  /* Inequality constraints */
  prob->F = F;
  
  if (F == NULL){
    prob->inequality_count = 0;
  }else{
    prob->inequality_count = F->rows;
  }
  /* Right hand side for F */
  prob->g = g;


  /* create sparse matrices */
  size_t n = matrix_sparsity(Q);
  if (n < Q->size/4) {
    prob->is_sparse = true;
    prob->sparse_Q = create_sparse_matrix(Q, n);
    prob->sparse_Q_inv = create_sparse_matrix(Q_inv, n);
  } else {
    prob->is_sparse = false;
  }


  /* insert all constraint into big matrix */
  fill_constraint_matrices(prob);
  

  /* Points and vectors */  
  if (z0 == NULL){
    prob->has_start_point = false;
    prob->z0 = create_matrix(prob->variable_count, 1);
  }else{
    prob->has_start_point = true;
    prob->z0 = z0;
  }  

  prob->z = create_matrix(prob->variable_count, 1);
  prob->has_solution = false;
  prob->solution = create_matrix(prob->variable_count, 1);

  prob->p = create_matrix(prob->variable_count, 1);
  prob->gk = create_matrix(prob->variable_count, 1);

  /* Work set */
  prob->active_set = work_set_create(prob->constraints_count);

  /* Max iterations */
  /* 0 is unlimited */
  prob->max_iter = max_iter;

  /* Max microseconds */
  if (max_micro_sec != 0){
    prob->check_time = true;
    prob->max_micro_sec = max_micro_sec;
  }else{
    prob->check_time = false;
  }

  return prob;
}

/* Prints the problem */
void print_problem(problem* prob){
  printf("\n********** Quadratic Problem **********\n\n");

  printf("Objective function: \n");
  printf("Q = \n");
  print_matrix(prob->Q);
  printf("q = \n");
  print_matrix(prob->q);

  printf("Equality constraints: \n");
  if (prob->E != NULL){
    printf("E = \n");
    print_matrix(prob->E);
    printf("h = \n");
    print_matrix(prob->h);
  }else{
    printf("-\n\n");
  }

  printf("Inequality constraints: \n");
  if (prob->F != NULL){
    printf("F = \n");
    print_matrix(prob->F);
    printf("g = \n");
    print_matrix(prob->g);
  }else{
    printf("-\n\n");
  }

  printf("Properties: \n");
  printf("Number of variables = %d\n", prob->z->rows);
  printf("Number of constraints = %d\n", prob->constraints_count);
  printf("Accuracy = %f\n\n", 1-prob->accuracy);

  printf("Starting point: \n");
  if (prob->has_start_point){    
    printf("z = \n");
    print_matrix(prob->z0);
  }else{
    printf("Not set or calculated yet.\n\n");
  }

  printf("Solution: \n");
  if (prob->has_solution){
    printf("z = \n");
    print_matrix(prob->solution);
    printf("value = %f\n", prob->solution_value);
  }else{
    printf("Not calculated yet.\n\n");
  }

  printf("***************************************\n");
}

/* Deallocates all the problems resources */
void free_problem(problem* prob){
  free_matrix(prob->Q);
  free_matrix(prob->Q_inv);

  free_matrix(prob->q);

  free_matrix(prob->E);
  free_matrix(prob->h);

  free_matrix(prob->F);
  free_matrix(prob->g);
  

  free_matrix(prob->A);
  free_matrix(prob->b);
  free_matrix(prob->lagrange);
  

  free_matrix(prob->z0);

  free_matrix(prob->z);

  free_matrix(prob->solution);

  free_matrix(prob->p);
  free_matrix(prob->gk);
  

  free_sparse_matrix(prob->sparse_Q);
  free_sparse_matrix(prob->sparse_Q_inv);
  int r;
  if (prob->is_sparse) {
    for (r = 0; r < prob->constraints_count; r++) {
      free_sparse_matrix(prob->sparse_A[r]);
    }
    free(prob->sparse_A);
  }

  work_set_free(prob->active_set);

  free(prob);
}

/* Returns a matrix with the currently active constraints */
matrix* get_active_conditions(problem* prob){
  matrix* A = create_matrix(prob->active_set->count, prob->A->columns);
  
  bool success = false;
  int i;
  for(i = 0; i < prob->active_set->count; i++){
    matrix* temp_row = get_row_vector_with_return(prob->active_set->data[i], prob->A);
    success = insert_row_vector(i+1, temp_row, A);
    free_matrix(temp_row);
  }

  if(success){
    return A;
  }else{
    free_matrix(A);
    return NULL;
  }
}

/* Returns a sparse matrix with the currently active constraints */
sparse_matrix* get_sparse_active_conditions(problem* prob){
  if (prob->active_set->count == 0) return NULL;

  int r, c, s = 0;

  /* count elements != 0 */
  for (r = 0; r < prob->active_set->count; r++) {
    s += prob->sparse_A[prob->active_set->data[r]-1]->size;
  }

  /* create sparse matrix */
  int dest = 0;
  sparse_matrix* S = create_empty_sparse_matrix(s);
  for (r = 0; r < prob->active_set->count; r++) {
    
    /* copy values */
    memcpy(S->A+dest, prob->sparse_A[prob->active_set->data[r]-1]->A, prob->sparse_A[prob->active_set->data[r]-1]->size*sizeof(value));
    /* copy columns */
    memcpy(S->cA+dest, prob->sparse_A[prob->active_set->data[r]-1]->cA, prob->sparse_A[prob->active_set->data[r]-1]->size*sizeof(int));
    /* copy rows */
    for (c = 0; c < prob->sparse_A[prob->active_set->data[r]-1]->size; c++) {
      S->rA[dest+c] = r+1;
    }
    /* move dest */
    dest += prob->sparse_A[prob->active_set->data[r]-1]->size;
  }

  S->rows = prob->active_set->count;
  S->columns = prob->variable_count;

  return S;


}

/* Returns a matrix with the right hand side of the currently active constraints */
matrix* get_active_conditions_rhs(problem* prob){
  matrix* b = create_matrix(prob->active_set->count, prob->b->columns);
  
  bool success = false;
  int i;
  for(i = 0; i < prob->active_set->count; i++){
    matrix* temp_row = get_row_vector_with_return(prob->active_set->data[i], prob->b);
    success = insert_row_vector(i+1, temp_row, b);
    free_matrix(temp_row);
  }

  if(success){
    return b;
  }else{
    free_matrix(b);
    return NULL;
  }
}

/* Calculates the optimum value given by the solution point */
bool get_solution_value(problem* prob){
  if(!prob->has_solution) return false;
  if (prob->is_sparse) {
    matrix* Qz = multiply_sparse_matrix_matrix(prob->sparse_Q, prob->z);
    prob->solution_value = dot_product(prob->z, Qz) + dot_product(prob->q, prob->z);
    free_matrix(Qz);
  } else {
    matrix* Qz = create_matrix(prob->z->rows, prob->z->columns);
    multiply_matrices(prob->Q, prob->z, Qz);
    prob->solution_value = dot_product(prob->z, Qz) + dot_product(prob->q, prob->z);
    free_matrix(Qz);  
  }

  return true;
}

/* Prints optimal point and optimal value */
void print_solution(problem* prob){
  printf("----------------Solution----------------\n");
  if (prob->has_solution){
    printf("z = \n");
    print_matrix(prob->solution);
    printf("value = %f\n", prob->solution_value);
  }else{
    printf("Not calculated yet.\n\n");
  }
}

/* Exits solver if maximal iterations or microseconds have been fullfilled */
bool time_to_exit(problem* prob, double time_spent){
  /* Exit if maximal iterations have been fullfilled */
  if (prob->max_iter == 1){
    return true;
  }
  prob->max_iter--;

  /* Exit if maximal micro seconds have been fullfilled */
  if (prob->check_time && time_spent >= prob->max_micro_sec){
    return true;
  }

  /* None have been fullfilled */
  return false;
}
