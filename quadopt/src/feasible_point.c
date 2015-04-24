/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-04-21
  Description: This file containts the necessary functions to determine if a point is feasible.
               It also contains functions that can calculate a feasible point with given constraints.
*/

#include <feasible_point.h>

void comb(int pool, int need, int* rows, int at, int ri, problem* prob, 
	  matrix* A, matrix* b, matrix* z, bool* done);

/* Checks if a point is feasible subject to the constraints in a problem */
bool is_feasible_point(matrix* z, problem* prob){
  value ans;
  int r, c;
  
  /* Check all equality constraints */
  for (r = 1; r <= prob->equality_count; r++){
    ans = 0;
    for (c = 1; c <= prob->E->columns; c++){
      ans += get_value_without_check(r, c, prob->E)*get_value(c, 1, z);
    }
    if (compare_elements(ans, get_value_without_check(r, 1, prob->h)) != 0){
      return false;
    }    
  }

  /* Check all inequality constraints */
  for (r = 1; r <= prob->inequality_count; r++){
    ans = 0;    
    for (c = 1; c <= prob->F->columns; c++){
      ans += get_value_without_check(r, c, prob->F)*get_value(c, 1, z);
    }
    
    if (compare_elements(ans, get_value_without_check(r, 1, prob->g)) == -1){
      return false;
    }
  }
  
  return true;
}

void rref(matrix* M, matrix* N) {
  int lead = 1;
  int i = 1;

  //TODO  doesn't need all theese
  //      fix indentation
  matrix* row1;
  matrix* row2;
  matrix* row3;

  for (int r = 1; r <= M->rows; r++) {
    if (M->columns+1 <= lead) {
      return;
    }
    i = r;
    while (compare_elements(get_value_without_check(i,lead,M),0) == 0) {
      i = i + 1;
      if (M->rows+1 == i) {
        i = r;
        lead = lead + 1;
        if (M->columns+1 == lead) {
          return;
        }
      }
    }
    switch_rows(i,r,M);
    switch_rows(i,r,N);
    if (compare_elements(get_value_without_check(r,lead,M),0) != 0) {
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,M);
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,N);
    }
    for (i = 1; i <= M->rows; i++) {
      if (i != r) {
        row1 = get_row_vector_with_return(r,M);
        row2 = get_row_vector_with_return(i,M);
        value val = -get_value_without_check(i,lead,M);
        multiply_matrix_with_scalar(val,row1);
        row3 = add_matrices_with_return(row1,row2);
        insert_row_vector(i,row3,M);

        free_matrix(row1);        
        free_matrix(row2);
        free_matrix(row3);

        row1 = get_row_vector_with_return(r,N);
        row2 = get_row_vector_with_return(i,N);
        multiply_matrix_with_scalar(val,row1);
        row3 = add_matrices_with_return(row1,row2);
        insert_row_vector(i,row3,N);

        free_matrix(row1);
        free_matrix(row2);
        free_matrix(row3);

      }
    }
    lead++;
  }
}

void rref1(matrix* M) {
  int lead = 1;
  int i = 1;

  //TODO  doesn't need all theese
  //      fix indentation
  matrix* row1 = create_matrix(1,M->columns);
  matrix* row2 = create_matrix(1,M->columns);
  matrix* row3 = create_matrix(1,M->columns);

  for (int r = 1; r <= M->rows; r++) {
    if (M->columns+1 <= lead) {
      free_matrix(row1);
      free_matrix(row2);
      free_matrix(row3);
      return;
    }
    i = r;
    while (compare_elements(get_value_without_check(i,lead,M),0) == 0) {
      i = i + 1;
      if (M->rows+1 == i) {
        i = r;
        lead = lead + 1;
        if (M->columns+1 == lead) {
          free_matrix(row1);
          free_matrix(row2);
          free_matrix(row3);
          return;
        }
      }
    }
    switch_rows(i,r,M);
    if (compare_elements(get_value_without_check(r,lead,M),0) != 0) {
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,M);
    }
    for (i = 1; i <= M->rows; i++) {
      if (i != r) {
        get_row_vector(r,M,row1);
        get_row_vector(i,M,row2);
        multiply_matrix_with_scalar(-get_value_without_check(i,lead,M),row1);
        add_matrices(row1,row2,row3);
        insert_row_vector(i,row3,M);
      }
    }
    lead++;
  }
  free_matrix(row1);
  free_matrix(row2);
  free_matrix(row3);
}


/* Iterate through all possible combinations of inequality constraints to add */
void comb(int pool, int need, int* rows, int at, int ri, problem* prob, matrix* A, matrix* b, matrix* z, bool* done){
  /* No more slot */
  if (pool < need + at || *done) return;
 
  if (need == 0){
    matrix* fi;
    int i;
    
    /* Add constraints */
    for (i = 0; i < ri; i++){
      fi = get_row_vector_with_return(rows[i]+1, prob->F);
      insert_row_vector(i+prob->equality_count+1, fi, A);
      free_matrix(fi);
      insert_value_without_check(get_value_without_check(rows[i]+1, 1, prob->g), 
				 i+prob->equality_count+1, 1, b);
    }
    
    /* If solution is feasible, return */
    if (solve_linear(A, z, b)){
      if (is_feasible_point(z, prob)){
        *done = true;
      }
    }
    return;
  }

  /* If we choose the current item, "or" (|) the bit to mark it so. */
  rows[need-1] = at;
  comb(pool, need - 1, rows, at + 1, ri, prob, A, b, z, done);
  /* Or don't choose it, go to next */
  comb(pool, need, rows, at + 1, ri, prob, A, b, z, done);
}

/* Calculates a feasible starting point for a problem */
bool find_starting_point(problem* prob){
  if (prob->equality_count > 0 || prob->inequality_count > 0){    

    /* Variables */
    matrix* A = create_matrix(prob->z->rows, prob->z->rows);
    matrix* b = create_matrix(prob->z->rows, 1);
    matrix* tmp_A;
    value tmp_b;    
    work_set* leads = work_set_create(prob->z->rows);
    work_set* vars_to_zero = work_set_create(prob->z->rows);
    matrix* Er = matrix_copy(prob->E);
    matrix* hr = matrix_copy(prob->h);

    /* reduce E so lead values can be retrieved */
    rref(Er,hr);

    /* get leads */
    int curr = 1;
    for (int r = 1; r <= Er->rows; r++) {
      for (int c = curr; c <= Er->columns; c++) {
        if (compare_elements(get_value_without_check(r, c, Er),1) == 0) {
          work_set_append(leads,c);
          curr = c+1;
          break;
        }
      }
    }

    /* get variables that can set to 0 later */
    for (int v = 1; v <= prob->z->rows; v++) {
      if (!work_set_contains(leads, v)) {
        work_set_append(vars_to_zero,v);
      }
    }

    /* Fill A and b with equality constraints */
    int r;
    for (r = 1; r <= prob->equality_count; r++){
      tmp_A = get_row_vector_with_return(r, prob->E);
      insert_row_vector(r, tmp_A, A);
      free_matrix(tmp_A);
      tmp_b = get_value_without_check(r, 1, prob->h);
      insert_value_without_check(tmp_b, r, 1, b);
    }

    int pool = prob->inequality_count;    
    int need = A->rows-prob->equality_count;

    int* rows = malloc(need*sizeof(int));
    bool done = false;

    matrix* new_row = create_matrix(1,A->columns);
    int count = 0;

    /* find feasible point */
    while (need > 0) {

      /* iterate through all combinations of inequality constraints */
      comb(pool, need, rows, 0, need, prob, A, b, prob->z0, &done);

      if (done) {
        break;
      } else {
        free(rows);        

        /* set one variable to 0 */
        if (count >= vars_to_zero->count) {
          for (int c = 1; c <= A->columns; c++) {
            insert_value_without_check(0, 1, c, new_row);          
          }
          /* insert this row for good */
          insert_value_without_check(1, 1, vars_to_zero->data[0], new_row);          
          insert_row_vector(A->rows-need, new_row, A);
          insert_value_without_check(0, A->rows-need, 1, b);

          /* insert next row */
          insert_value_without_check(0, 1, vars_to_zero->data[0], new_row);
          work_set_remove(vars_to_zero,vars_to_zero->data[0]);
          count = 1;
          need--;
          insert_value_without_check(1, 1, vars_to_zero->data[0], new_row);
          insert_row_vector(A->rows-need, new_row, A);
          insert_value_without_check(0, A->rows-need, 1, b);

        } else {
          if (count == 0) {
            need--;
          }
          for (int c = 1; c <= A->columns; c++) {
            insert_value_without_check(0, 1, c, new_row);          
          }
          insert_value_without_check(1, 1, vars_to_zero->data[count], new_row);
          insert_row_vector(A->rows-need, new_row, A);
          insert_value_without_check(0, A->rows-need, 1, b);
          count++;
        }   

        rows = malloc(need*sizeof(int));

      }
    }
/*
    print_matrix(A);
    print_matrix(b);

    printf("%s", done ? "true\n" : "false\n");*/

    if (!done) {
      //Shall use linear_solve here
      //done = solve_linear(A, prob->z0, b);
      least_square(A, prob->z0, b);
      done = true;
    }
    
    free(rows);
    free_matrix(A);
    free_matrix(b);

    return done;
  }else{
    /* problem is unconstrained, any point is feasible */
    prob->z0 = get_zero_matrix(prob->z->rows, prob->z->columns);
    return true;
  }
}
