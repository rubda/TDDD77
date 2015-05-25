/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-05-04
  Description: Simplex method to find a starting point.
*/

#include <simplex.h>

bool is_neg_tableau_row(int row, matrix* tableau);
int min_test(int column, matrix* tableau);

bool simplex_phase_1(problem* prob){
  /* Slack and virtual variable sets */  
  work_set* virtual_vars = work_set_create(prob->equality_count+prob->inequality_count);

  matrix* Fr;  
  matrix* gr;

  matrix* Ft;
  matrix* Et;

  size_t r, i, c;

  /* Negatate all equality constraints where RHS is negative */
  if (prob->equality_count > 0){  
    for (r = 1; r <= prob->equality_count; r++){
      if (compare_elements(get_value_without_check(r, 1, prob->h), 0) == -1){
        multiply_row_with_scalar(-1, r, prob->E);
        multiply_row_with_scalar(-1, r, prob->h);
      }
      /* Add virtual variable */
      work_set_append(virtual_vars, r);
    }
  }


  /* Convert >= to <= */
  if (prob->inequality_count > 0){
    Fr = matrix_copy(prob->F);
    multiply_matrix_with_scalar(-1, Fr);
    gr = matrix_copy(prob->g);
    multiply_matrix_with_scalar(-1, gr);

    /* Find last virtual variable */
    for (r = 1; r <= prob->inequality_count; r++){
      if (compare_elements(get_value_without_check(r, 1, gr), 0) == -1){
        work_set_append(virtual_vars, r+prob->equality_count);
      }
    }
  }

  /* Split variables to allow negative values */
  if (prob->inequality_count > 0) {
    Ft = create_matrix(prob->inequality_count, prob->variable_count * 2);
    matrix* temp = matrix_copy(Fr);
    insert_sub_matrix(1, prob->inequality_count, 1, prob->variable_count, temp, Ft);
    multiply_matrix_with_scalar(-1, temp);
    insert_sub_matrix(1, prob->inequality_count, prob->variable_count+1, Ft->columns, temp, Ft);

    free_matrix(temp);
    free_matrix(Fr);
  }

  /* Split variables to allow negative values */
  if (prob->equality_count > 0){
    Et = create_matrix(prob->equality_count, prob->variable_count * 2);
    matrix* temp = matrix_copy(prob->E);
    insert_sub_matrix(1, prob->equality_count, 1, prob->variable_count, temp, Et);
    multiply_matrix_with_scalar(-1, temp);
    insert_sub_matrix(1, prob->equality_count, prob->variable_count+1, Et->columns, temp, Et);

    free_matrix(temp);
  }

  /* Initial tableau structure example
   *
   * Constraints
   * 1*x1 - 1*x2 + 2*x3  = 0
   * 2*x1 + 1*x2 + 5*x3  = 0
   * 1*x1 - 1*x2 + 2*x3 <= 1
   * 2*x1 + 2*x2 + 2*x3 <= -2
   * 1*x1 + 2*x2 + 3*x3 <= 2
   *
   * Slack and virtual variables added
   * 1*x1 - 1*x2 + 2*x3                      + 1*a1                = 0
   * 2*x1 + 1*x2 + 5*x3                             + 1*a2         = 0
   * 1*x1 - 1*x2 + 2*x3 + 1*s1                                     = 1
   * 2*x1 + 2*x2 + 2*x3        + 1*s2                      - 1*a3  = -2
   * 1*x1 + 2*x2 + 3*x3               + 1*s3                       = 2 
   *
   * Constructed tableau
   *     | x1  x2  x3  s1  s2  s3  a1  a2  a3 | rhs
   * ------------------------------------------------
   * a1  | 1   -1  2   0   0   0   1   0   0  | 0
   * a2  | 2   1   5   0   0   0   0   1   0  | 0
   * s1  | 1   -1  2   1   0   0   0   0   0  | 1
   * s2  | 2   2   2   0   1   0   0   0   -1 | -2
   * s3  | 1   2   3   0   0   1   0   0   0  | 2
   * -----------------------------------------------
   * z   | 0   0   0   0   0   0   -1  -1  -1 | 0
   *
   *
   */

  /* Create tableau. +1 rows for objective function, +1 columns for RHS. */
  matrix* tableau = get_zero_matrix(prob->inequality_count + prob->equality_count + 1, prob->variable_count*2 +
				    prob->inequality_count + virtual_vars->count + 1);  

  /* Intial basis */
  work_set* basis = work_set_create(prob->equality_count+prob->inequality_count);
  /* insert virtual vars first */
  for (i = 1; i <= prob->equality_count; i++){
    work_set_append(basis, i+prob->variable_count*2+prob->inequality_count);
  }

  /* Insert slack vars */
  for (i = 1; i <= prob->inequality_count; i++){
    work_set_append(basis, i+prob->variable_count*2);
  }

  /* Insert equality constraints */
  if (prob->equality_count > 0){
    insert_sub_matrix(1, prob->equality_count, 1, prob->variable_count*2, Et, tableau);
    insert_sub_matrix(1, prob->equality_count, tableau->columns, tableau->columns, prob->h, tableau);
    
    free_matrix(Et);
  }

  /* Insert inequality constraints */
  if (prob->inequality_count > 0){
    insert_sub_matrix(prob->equality_count + 1, prob->equality_count + prob->inequality_count, 
		      1, prob->variable_count*2, Ft, tableau);

    insert_sub_matrix(prob->equality_count + 1, prob->equality_count + prob->inequality_count,
		      tableau->columns, tableau->columns, gr, tableau);
   
    free_matrix(Ft);
    free_matrix(gr);
  }

  /* Insert virtual variables */
  for (r = 1; r <= virtual_vars->count; r++){
    if (virtual_vars->data[r-1] > prob->equality_count){
      insert_value_without_check(-1, virtual_vars->data[r-1], prob->variable_count*2 + prob->inequality_count + r, tableau);
    }else{
      insert_value_without_check(1, virtual_vars->data[r-1], prob->variable_count*2 + prob->inequality_count + r, tableau);
    }  
  }
  work_set_free(virtual_vars);

  /* Insert slack variables */
  for (r = prob->equality_count+1; r < tableau->rows; r++){
    insert_value_without_check(1, r, prob->variable_count*2 + (r-prob->equality_count), tableau);
  }

  /* Insert objective function: min sum of virtual variables */
  for (c = prob->variable_count*2 + prob->inequality_count + 1; c < tableau->columns; c++){
    insert_value_without_check(-1, tableau->rows, c, tableau);
  }
  
  /* Adjust objective so it does not contain any virtual variables */
  /* Remove vars from equality constraints */
  for (r = 1; r <= prob->equality_count; r++){
    add_rows(r, tableau->rows, tableau);
  }
  
  /* Remove vars from inequality constraints */
  size_t temp_col = prob->variable_count*2 + prob->inequality_count + prob->equality_count + 1;

  for (r = prob->equality_count+1; r <= tableau->rows-1; r++){
    if (temp_col >= tableau->columns){
      break;
    } else if (compare_elements(get_value_without_check(r, temp_col, tableau), 0) == 0){
      continue;
    }
    /* Removes virtual variable from objective function */
    matrix* temp_row = get_row_vector_with_return(r, tableau);
    multiply_matrix_with_scalar(-1, temp_row);
    matrix* temp_obj = get_row_vector_with_return(tableau->rows, tableau);
    matrix* new_obj = add_matrices_with_return(temp_row, temp_obj);
    insert_row_vector(tableau->rows, new_obj, tableau);

    /* Free help matrices */
    free_matrix(temp_row);
    free_matrix(temp_obj);
    free_matrix(new_obj);

    /* Next var */
    temp_col++;
  }

  int column, row;
  value cur, last;
  bool error = false;

  /* Set z0 to 0 */
  for (r = 1; r <= prob->variable_count; r++){
    insert_value_without_check(0, r, 1, prob->z0);
  }

  /* Solve */ 
  while (!is_neg_tableau_row(tableau->rows, tableau)){
    column = -1;
    row = -1;
    cur = 0;
    last = 0;

    /* Find most positive element in objective */
    for (c = 1; c <= tableau->columns-1; c++){
      cur = get_value_without_check(tableau->rows, c, tableau);
      if (compare_elements(cur,last) == 1){        
        column = c;
        last = cur;
      }
    }

    /* Should not be able to happen */
    if (column == -1){
      error = true;
      break;
    }

    /* Run min test on choosen column */
    row = min_test(column, tableau);

    /* Infeasible problem */
    if (row == -1){
      error = true;
      break;
    }   

    /* Change basis */
    basis->data[row-1] = column;

    /* Get a 1 at choosen position */
    value div = get_value_without_check(row, column, tableau);
    divide_row_with_scalar(div, row, tableau);

    /* Eliminate other values in the column */
    value scal;
    matrix* pivot_row;
    matrix* old_row;
    matrix* new_row;

    for (r = 1; r <= tableau->rows; r++){
      if ((int)r == row){
        continue;
      }
      scal = -get_value_without_check(r, column, tableau);
      old_row = get_row_vector_with_return(r, tableau);

      pivot_row = get_row_vector_with_return(row, tableau);
      multiply_matrix_with_scalar(scal, pivot_row);

      new_row = add_matrices_with_return(pivot_row, old_row);

      insert_row_vector(r, new_row, tableau);

      free_matrix(pivot_row);
      free_matrix(old_row);
      free_matrix(new_row);
    }    
  }
  

  value val;
  if (!error) {
    /* Loop through basis to set variables */
    for (r = 1; r <= basis->count; r++) {
      if (basis->data[r-1] <= prob->variable_count) {
        val = get_value_without_check(basis->data[r-1], 1, prob->z0) + get_value_without_check(r, tableau->columns, tableau);
        insert_value_without_check(val, basis->data[r-1], 1, prob->z0);
      } else if (basis->data[r-1] > prob->variable_count && basis->data[r-1] <= prob->variable_count*2) {
        val = get_value_without_check(basis->data[r-1]-prob->variable_count, 1, prob->z0) - get_value_without_check(r, tableau->columns, tableau);
        insert_value_without_check(val, basis->data[r-1]-prob->variable_count, 1, prob->z0);
      }
    }
  }

  free_matrix(tableau);
  work_set_free(basis);
  return !error;
}

/* Checks if all the elements in a row in the simplex tableau is negative or zero */
bool is_neg_tableau_row(int row, matrix* tableau){
  size_t c;
  for (c = 1; c <= tableau->columns; c++){
    if (compare_elements(get_value_without_check(row, c, tableau), 0) == 1){
      return false;
    }
  }
  return true;
}

/* Returns the row with smallest RHS/(LHS element in choosen column), for simplex phase I*/
int min_test(int column, matrix* tableau){
  value temp, cur;
  bool first = true;
  int min_row = -1;
  size_t r;
  for (r = 1; r < tableau->rows; r++){
    temp = get_value_without_check(r, column, tableau);
    if (compare_elements(temp, 0) == 1){
      temp = get_value_without_check(r, tableau->columns, tableau)/temp;
      if (first){
        min_row = r;
        cur = temp;
        first = false;
      }else if (compare_elements(temp, cur) == -1){
        min_row = r;
        cur = temp;
      }
    }
  }
  return min_row;
}



