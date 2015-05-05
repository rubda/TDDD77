/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-04-21
  Description: This file contains the necessary functions to reach the optimum value given by the problem.
*/

#include <stdio.h>
#include <solver.h>
#include <math.h>
#include <feasible_point.h>
#include <subproblem.h>
#include <time.h>
#include <simplex.h>

bool fill_active_set(problem* prob);

bool take_step(problem* prob);

void copy_solution(problem* prob);

/* Fills the active set according to the current position */
bool fill_active_set(problem* prob){
  work_set_clear(prob->active_set);

  /* Fill */
  int i;
  for (i = 1; i <= prob->A->rows; i++){
    if (i <= prob->equality_count){
      work_set_append(prob->active_set, i);
      continue;
    }
    value ans = 0;

    int j;
    for (j = 1; j <= prob->A->columns; j++){
      ans += get_value(i, j, prob->A)*get_value(j, 1, prob->z); 
      /* TODO add check and get_value_without_check and return false */
    }

    if (compare_elements(ans, get_value_without_check(i, 1, prob->b)) == 0){
      work_set_append(prob->active_set, i);
    }
  }

  return true;
}

/* Removes the active constraint with the most negative lagrange multiplier */
bool remove_constraint(problem* prob){
  matrix* ait;
  matrix* ai;
  matrix* LA = create_matrix(prob->p->rows, prob->active_set->count);
  matrix* lagrange = create_matrix(prob->active_set->count, 1);

  int i;
  for (i = 1; i <= prob->active_set->count; i++) {
    ai = get_row_vector_with_return(prob->active_set->data[i-1], prob->A);
    ait = transpose_matrix_with_return(ai);
    insert_column_vector(i, ait, LA);
    free_matrix(ai);
    free_matrix(ait);
  }
  
  if (!gauss_jordan_solver(LA, lagrange, prob->gk)){
    least_square(LA, lagrange, prob->gk);
  }
  
  /* Find most negative and remove (if not equality constraint) */
  int small;
  value tmp;
  value val = 0;

  int j;
  for (j = 1; j <= lagrange->rows; j++){
    if (prob->active_set->data[j-1] <= prob->equality_count){
      continue;
    }
    tmp = get_value_without_check(j, 1, lagrange);
    if (tmp < val){
      small = prob->active_set->data[j-1];
      val = tmp;
    }
  }

  if (val < 0) {
    /* Remove */
    work_set_remove(prob->active_set,small);
    free_matrix(LA);
    free_matrix(lagrange);
    return true;
  }

  /* Could not remove any constraints */
  free_matrix(LA);
  free_matrix(lagrange);
  return false;
}

/* Calculates and takes the step for active set method */
bool take_step(problem* prob){
  matrix* ai, *ati;
  ati = create_matrix(prob->A->columns, 1);
  ai = create_matrix(1, prob->A->columns);
  matrix* z_old = matrix_copy(prob->z);
  value bi, nom, temp_step, step = 1;

  /* Only go through the inequality constraints */
  int i;
  for (i = 1; i <= prob->A->rows; i++){
    if (work_set_contains(prob->active_set, i)){
      continue;
    }
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
