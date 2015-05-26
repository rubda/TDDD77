/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to reach the optimum value given by the problem.
*/

#include <stdio.h>
#include <solver.h>
#include <math.h>
#include <subproblem.h>
#include <time.h>
#include <simplex.h>

bool fill_active_set(problem* prob);
bool take_step(problem* prob);
void copy_solution(problem* prob);


void prefill_set(problem* prob){
  size_t i;
  for (i = 1; i <= prob->equality_count; i++) {
    work_set_append(prob->active_set, i);
  }
}

/* Fills the active set according to the current position */
bool fill_active_set(problem* prob){
  prob->active_set->count = prob->equality_count;

  if (prob->is_sparse){
    /* Fill */
    size_t i;
    for (i = prob->equality_count; i < prob->constraints_count; i++){
      value ans = 0;    

      size_t j;
      for (j = 0; j < prob->sparse_A[i]->size; j++){
        ans += prob->sparse_A[i]->A[j]*get_value_without_check(prob->sparse_A[i]->cA[j], 1, prob->z);
      }

      if (compare_elements(ans, get_value_without_check(i, 1, prob->b)) == 0){
        work_set_append(prob->active_set, i);
      }
    }
  }else{
    /* Fill */
    size_t i;
    for (i = prob->equality_count+1; i <= prob->constraints_count; i++){
      value ans = 0;    

      size_t j;
      for (j = 1; j <= prob->variable_count; j++){
        ans += get_value(i, j, prob->A)*get_value_without_check(j, 1, prob->z);
      }

      if (compare_elements(ans, get_value_without_check(i, 1, prob->b)) == 0){
        work_set_append(prob->active_set, i);
      }
    }
  }

  return true;
}

/* Removes the active constraint with the most negative lagrange multiplier */
bool remove_constraint(problem* prob){
  /* Check if unconstrained problem */
  if (prob->constraints_count == 0){
    return false;
  }

  if (prob->lagrange == NULL){
    /* Calculate lagrange multiplicator */
    matrix* ait;
    matrix* ai;
    matrix* LA = create_matrix(prob->p->rows, prob->active_set->count);
    prob->lagrange = create_matrix(prob->active_set->count, 1);

    /* Create right and left hand side of system */
    size_t i;
    for (i = 1; i <= prob->active_set->count; i++){
      ai = get_row_vector_with_return(prob->active_set->data[i-1], prob->A);
      ait = transpose_matrix_with_return(ai);
      insert_column_vector(i, ait, LA);
      free_matrix(ai);
      free_matrix(ait);
    }
    
    /* Solve system to retrieve lagrange multiplicators */
    if (!gauss_jordan_solver(LA, prob->lagrange, prob->gk)){
      least_square(LA, prob->lagrange, prob->gk);
    }
    free_matrix(LA);
  }
  
  /* Find most negative and remove (if not equality constraint) */
  int small;
  value tmp;
  value val = 0;

  size_t j;
  for (j = 1; j <= prob->lagrange->rows; j++){
    if (prob->active_set->data[j-1] <= prob->equality_count){
      continue;
    }
    tmp = get_value_without_check(j, 1, prob->lagrange);
    if (tmp < val){
      small = prob->active_set->data[j-1];
      val = tmp;
    }
  }

  free_matrix(prob->lagrange);
  prob->lagrange = NULL;
  /* Check if value is negative */
  if (val < 0) {
    /* Remove */
    work_set_remove(prob->active_set,small);
    return true;
  }

  /* Could not remove any constraints */  
  return false;
}

/* Calculates and takes the step for active set method */
bool take_step(problem* prob){
  matrix* ai;
  matrix* ati;
  ati = create_matrix(prob->variable_count, 1);
  ai = create_matrix(1, prob->variable_count);
  matrix* z_old = matrix_copy(prob->z);
  value bi, nom, dnom, temp_step, step = 1;

  /* Only go through the inequality constraints */
  size_t i, j;
  bool cont;
  for (i = prob->equality_count+1; i <= prob->A->rows; i++){
    cont = false;
    for (j = prob->equality_count; j < prob->active_set->count; j++){
      if (prob->active_set->data[j] == i){
        cont = true;
        break;
      }
    }
    if (cont){
      continue;
    }

    if (prob->is_sparse){
      nom = 0;
      for (j = 0; j < prob->sparse_A[i-1]->size; j++){
        nom += prob->sparse_A[i-1]->A[j]*get_value_without_check(prob->sparse_A[i-1]->cA[j], 1, prob->p);
      }
      if (nom < 0){
        bi = get_value(i, 1, prob->b);
        dnom = 0;
        for (j = 0; j < prob->sparse_A[i-1]->size; j++){
          dnom += prob->sparse_A[i-1]->A[j]*get_value_without_check(prob->sparse_A[i-1]->cA[j], 1, prob->z);
        }
        temp_step = (bi - dnom)/nom;
        if (temp_step < step){
          step = temp_step;
        }
      }
    }else{
      get_row_vector(i, prob->A, ai);
      transpose_matrix(ai, ati);
      nom = dot_product(ati, prob->p);
      if (nom < 0){
        bi = get_value(i, 1, prob->b);
        temp_step = (bi - dot_product(ati, prob->z))/nom;
        if (temp_step < step){
          step = temp_step;
        }
      }
    }
  }

  /* Free matrices and return */
  if (compare_elements(step, 0) != 0){
    multiply_matrix_with_scalar(step, prob->p);    
    add_matrices(z_old, prob->p, prob->z);

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

/* Solves a quadratic problem using the active set method */
matrix* quadopt_solver(problem* prob){
  clock_t begin, end;
  double time_spent = 0;
  double tmp_spent = 0;

  /* Calculate starting point if no one is provide or the one provided is infeasible */
  if (!prob->has_start_point || !is_feasible_point(prob->z0, prob)){
    if (!simplex_phase_1(prob)){
      printf("No feasible point\n");
      return NULL;
    }
  }

  matrix_copy_data(prob->z0, prob->z);

  prefill_set(prob);
  fill_active_set(prob);

  while (true){
    begin = clock();
    solve_subproblem(prob);

    if (is_zero_matrix(prob->p)){
      if (prob->active_set->count == 0){
        break;
      }
      if (!remove_constraint(prob)){
        break;
      }
    }else{
      /* Could not move */
      if (!take_step(prob)){
        break;
      }
      /* Set active set */
      fill_active_set(prob);
    }

    end = clock();
    tmp_spent = (double)(end - begin) / (CLOCKS_PER_SEC/1000000);
    time_spent += tmp_spent;

    /* Exit if maximal iterations or microseconds have been fullfilled */
    if (time_to_exit(prob, time_spent)){
      break;
    }
  }

  copy_solution(prob);
  return prob->solution;
}

/* Copies the current point (z) to the solution matrix. */
void copy_solution(problem* prob){
  matrix_copy_data(prob->z, prob->solution);
  prob->has_solution = true;
  get_solution_value(prob);
}
