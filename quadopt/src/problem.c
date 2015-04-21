/*
  Author: Ruben Das
  Email: rubda680@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to retrieve values from the matrices given.
*/

#include <problem.h>

/* Allocates the problem and sets all necessary variables */
problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, 
			matrix* F, matrix* g, matrix* z0){

  problem* prob = malloc(sizeof(problem));

  /* Objective function */
  prob->Q = Q;
  matrix* Q_inv = create_matrix(Q->rows, Q->columns);
  get_inverse(Q, Q_inv);
  prob->Q_inv = Q_inv;

  prob->q = q;

  /* Constraints */

  /* Equality constraints */
  prob->E = E;
  /* Right hand side for E */
  prob->h = h;

  /* Inequality constraints */
  prob->F = F;
  /* Right hand side for F */
  prob->g = g;

  if (E == NULL){
    prob->equality_count = 0;
  }else{
    prob->equality_count = E->rows;
  }
  
  if (F == NULL){
    prob->inequality_count = 0;
  }else{
    prob->inequality_count = F->rows;
  }

  prob->constraints_count = prob->equality_count + prob->inequality_count;

  /* All constrains */
  prob->A = create_matrix(prob->constraints_count, q->rows);
  /* Right hand side of b */
  prob->b = create_matrix(prob->constraints_count, 1);
  
  /* Insert equality constraints */
  matrix* temp_row;
  int r;
  for (r = 1; r <= prob->equality_count; r++){
    temp_row = get_row_vector_with_return(r, E);
    insert_row_vector(r, temp_row, prob->A);
    insert_value_without_check(get_value_without_check(r, 1, h), r, 1, prob->b);
    free_matrix(temp_row);
  }

  /* Insert inequality constraints */
  for (r = 1; r <= prob->inequality_count; r++){
    temp_row = get_row_vector_with_return(r, F);
    insert_row_vector(r+prob->equality_count, temp_row, prob->A);
    insert_value_without_check(get_value_without_check(r, 1, g), r+prob->equality_count, 1, prob->b);
    free_matrix(temp_row);
  }

  /* Points and vectors */  
  if (z0 == NULL){
    prob->has_start_point = false;
    prob->z0 = create_matrix(q->rows, 1);
  }else{
    prob->has_start_point = true;
    prob->z0 = z0;
  }  

  prob->z = create_matrix(q->rows, 1);
  prob->has_solution = false;
  prob->solution = create_matrix(q->rows, 1);

  prob->lagrange = create_matrix(prob->A->rows, 1);
  prob->p = create_matrix(q->rows, 1);
  prob->gk = create_matrix(q->rows, 1);

  /* Work set */
  prob->active_set = work_set_create(prob->A->rows);

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

  free_matrix(prob->z0);

  free_matrix(prob->z);

  free_matrix(prob->solution);

  free_matrix(prob->p);
  free_matrix(prob->gk);
  free_matrix(prob->lagrange);

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

  matrix* z_trans = create_matrix(prob->z->columns, prob->z->rows);
  transpose_matrix(prob->z, z_trans);

  matrix* q_trans = create_matrix(prob->q->columns, prob->q->rows);
  transpose_matrix(prob->q, q_trans);

  matrix* zTQ = multiply_matrices_with_return(z_trans, prob->Q);
  matrix* zTQz = multiply_matrices_with_return(zTQ, prob->z);
  multiply_matrix_with_scalar(0.5, zTQz);
  matrix* qTz = multiply_matrices_with_return(q_trans, prob->z);
  matrix* solution = add_matrices_with_return(zTQz, qTz);

  prob->solution_value = get_value_without_check(1, 1, solution);

  free_matrix(z_trans);
  free_matrix(q_trans);
  free_matrix(zTQ);
  free_matrix(zTQz);
  free_matrix(qTz);
  free_matrix(solution);

  return true;
}
