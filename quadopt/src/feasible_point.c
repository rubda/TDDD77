/*
  Author: Johan Isaksson
  Email: johis024@student.liu.se
  Date: 2015-04-21
  Description: This file containts the necessary functions to determine if a point is feasible.
               It also contains functions that can calculate a feasible point with given constraints.
*/

#include <feasible_point.h>

void comb(int pool, int need, int* rows, int at, int ri, problem* prob, matrix* F, matrix* g, 
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

void rref(matrix* M, matrix* N){
  int lead = 1;
  int i = 1;

  //TODO  doesn't need all theese
  //      fix indentation
  matrix* row1;
  matrix* row2;
  matrix* row3;

  for (int r = 1; r <= M->rows; r++){
    if (M->columns+1 <= lead){
      return;
    }
    i = r;
    while (compare_elements(get_value_without_check(i,lead,M),0) == 0){
      i = i + 1;
      if (M->rows+1 == i){
        i = r;
        lead = lead + 1;
        if (M->columns+1 == lead){
          return;
        }
      }
    }
    switch_rows(i,r,M);
    switch_rows(i,r,N);
    if (compare_elements(get_value_without_check(r,lead,M),0) != 0){
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,M);
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,N);
    }
    for (i = 1; i <= M->rows; i++){
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

  for (int r = 1; r <= M->rows; r++){
    if (M->columns+1 <= lead){
      free_matrix(row1);
      free_matrix(row2);
      free_matrix(row3);
      return;
    }
    i = r;
    while (compare_elements(get_value_without_check(i,lead,M),0) == 0){
      i = i + 1;
      if (M->rows+1 == i){
        i = r;
        lead = lead + 1;
        if (M->columns+1 == lead){
          free_matrix(row1);
          free_matrix(row2);
          free_matrix(row3);
          return;
        }
      }
    }
    switch_rows(i,r,M);
    if (compare_elements(get_value_without_check(r,lead,M),0) != 0){
      divide_row_with_scalar(get_value_without_check(r,lead,M),r,M);
    }
    for (i = 1; i <= M->rows; i++){
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
void comb(int pool, int need, int* rows, int at, int ri, problem* prob, matrix* F, matrix* g, matrix* A, matrix* b, matrix* z, bool* done){
  /* No more slot */
  if (pool < need + at || *done) return;
 
  if (need == 0){
    matrix* fi;
    int i;
    
    /* Add constraints */
    for (i = 0; i < ri; i++){
      fi = get_row_vector_with_return(rows[i]+1, F);
      insert_row_vector(i+prob->equality_count+1, fi, A);
      free_matrix(fi);
      insert_value_without_check(get_value_without_check(rows[i]+1, 1, g), 
				 i+prob->equality_count+1, 1, b);
    }
    
    /* If solution is feasible, return */
    if (gauss_jordan_solver(A, z, b)){
      if (is_feasible_point(z, prob)){
        *done = true;
      }
    }
    return;
  }

  /* If we choose the current item, "or" (|) the bit to mark it so. */
  rows[need-1] = at;
  comb(pool, need - 1, rows, at + 1, ri, prob, F, g, A, b, z, done);
  /* Or don't choose it, go to next */
  comb(pool, need, rows, at + 1, ri, prob, F, g, A, b, z, done);
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

    /* Reduce E so lead values can be retrieved */
    rref(Er, hr);

    /* Get leads */
    int curr = 1;
    int r, c;
    for (r = 1; r <= Er->rows; r++){
      for (c = curr; c <= Er->columns; c++){
        if (compare_elements(get_value_without_check(r, c, Er),1) == 0){
          work_set_append(leads,c);
          curr = c+1;
          break;
        }
      }
    }

    /* Get variables that can set to 0 later */
    int v;
    for (v = 1; v <= prob->z->rows; v++){
      if (!work_set_contains(leads, v)){
        work_set_append(vars_to_zero,v);
      }
    }

    /* Create new pool of possible constraints */
    matrix* Fn = create_matrix(prob->inequality_count+vars_to_zero->count, prob->Q->columns);
    matrix* gn = create_matrix(prob->inequality_count+vars_to_zero->count, prob->q->columns);

    /* Copy all the old data */
    matrix* row;
    for (r = 1; r <= prob->inequality_count; r++){
      row = get_row_vector_with_return(r, prob->F);
      insert_row_vector(r, row, Fn);
      free_matrix(row);
      row = get_row_vector_with_return(r, prob->g);
      insert_row_vector(r, row, gn);
      free_matrix(row);
    }

    /* Insert vars set to 0 */
    row = get_zero_matrix(1, prob->Q->columns);
    for (r = prob->inequality_count+1; r <= Fn->rows; r++){
      insert_value_without_check(1, 1, vars_to_zero->data[r-prob->inequality_count-1], row);
      insert_row_vector(r, row, Fn);
      insert_value_without_check(0, 1, vars_to_zero->data[r-prob->inequality_count-1], row);
      insert_value_without_check(0, r, 1, gn);
    }

    /* Fill A and b with equality constraints */
    for (r = 1; r <= prob->equality_count; r++){
      tmp_A = get_row_vector_with_return(r, prob->E);
      insert_row_vector(r, tmp_A, A);
      free_matrix(tmp_A);
      tmp_b = get_value_without_check(r, 1, prob->h);
      insert_value_without_check(tmp_b, r, 1, b);
    }

    int pool = prob->inequality_count+vars_to_zero->count;    
    int need = A->rows-prob->equality_count;

    int* rows = malloc(need*sizeof(int));
    bool done = false;

    /* Find a feasible point */
    comb(pool, need, rows, 0, need, prob, Fn, gn, A, b, prob->z0, &done);

    free(rows);
    free_matrix(row);
    free_matrix(A);
    free_matrix(b);
    free_matrix(Er);
    free_matrix(hr);
    free_matrix(Fn);
    free_matrix(gn);
    work_set_free(leads);
    work_set_free(vars_to_zero);

    return done;
  }else{
    /* Problem is unconstrained, any point is feasible */
    prob->z0 = get_zero_matrix(prob->z->rows, prob->z->columns);
    return true;
  }
}














/* checks if all the elements in a row in the simplex tableau is negative or zero */
bool is_neg_tableau_row(int row, matrix* tableau) {
  for (int c = 1; c <= tableau->columns-1; c++) {
    if (compare_elements(get_value_without_check(row, c, tableau),0) == 1) {
      return false;
    }
  }
  return true;
}

/* returns the row with smallest rhs/(lhs element in choosen column), for simplex phase I*/
int min_test(int column, matrix* tableau) {
  value temp, cur;
  bool first = true;
  int min_row = -1;
  for (int r = 1; r < tableau->rows; r++) {
    temp = get_value_without_check(r, column, tableau);
    if (compare_elements(temp,0) == 1) {
      temp = get_value_without_check(r, tableau->columns, tableau)/temp;
      if (first) {
        min_row = r;
        cur = temp;
        first = false;
      } else if (compare_elements(temp, cur) == -1) {
        min_row = r;
        cur = temp;
      }
    }
  }
  return min_row;
}



bool simplex_phase_1(problem* prob) {


  /* slack and virtual variable sets */  
  work_set* virtual_vars = work_set_create(prob->equality_count+prob->equality_count);

  matrix* Fr;  
  matrix* gr;

  matrix* Ft;
  matrix* Et;


  /* negatate all equality constraints where rhs is negative */
  if (prob->equality_count > 0) {  
    for (int r = 1; r <= prob->equality_count; r++) {
      if (compare_elements(get_value_without_check(r, 1, prob->h),0) == -1) {
        multiply_row_with_scalar(-1, r, prob->E);
        multiply_row_with_scalar(-1, r, prob->h);
      }
      /* add virtual variable */
      work_set_append(virtual_vars, r);
    }
  }


  /* convert >= to <= */
  if (prob->inequality_count > 0) {
    Fr = matrix_copy(prob->F);
    multiply_matrix_with_scalar(-1, Fr);
    gr = matrix_copy(prob->g);
    multiply_matrix_with_scalar(-1, gr);

    /* find last virtual variable */
    for (int r = 1; r <= prob->inequality_count; r++) {
      //print_matrix(gr);
      if (compare_elements(get_value_without_check(r, 1, gr), 0) == -1) {
        //printf("row: %d\n", r);
        work_set_append(virtual_vars, r+prob->equality_count);
      }
    }
  }


  /* split variables to allow negative values */
  if (prob->inequality_count > 0) {
    Ft = create_matrix(prob->inequality_count, prob->variable_count * 2);
    matrix* temp = matrix_copy(Fr);
    insert_sub_matrix(1, prob->inequality_count, 1, prob->variable_count, temp, Ft);
    multiply_matrix_with_scalar(-1, temp);
    insert_sub_matrix(1, prob->inequality_count, prob->variable_count+1, Ft->columns, temp, Ft);

    free_matrix(temp);
    free_matrix(Fr);
    //print_matrix(Ft);
  }

  /* split variables to allow negative values */
  if (prob->equality_count > 0) {
    Et = create_matrix(prob->equality_count, prob->variable_count * 2);
    matrix* temp = matrix_copy(prob->E);
    insert_sub_matrix(1, prob->equality_count, 1, prob->variable_count, temp, Et);
    multiply_matrix_with_scalar(-1, temp);
    insert_sub_matrix(1, prob->equality_count, prob->variable_count+1, Ft->columns, temp, Et);

    free_matrix(temp);
    //print_matrix(Et);
  }

  //work_set_print(virtual_vars);

  /* initial tableau structure example
  *
  * Constraints
  * 1*x1 - 1*x2 + 2*x3  = 0
  * 2*x1 + 1*x2 + 5*x3  = 0
  * 1*x1 - 1*x2 + 2*x3 <= 1
  * 2*x1 + 2*x2 + 2*x3 <= -2
  * 1*x1 + 2*x2 + 3*x3 <= 2
  *
  * slack and virtual variables added
  * 1*x1 - 1*x2 + 2*x3                      + 1*a1                = 0
  * 2*x1 + 1*x2 + 5*x3                             + 1*a2         = 0
  * 1*x1 - 1*x2 + 2*x3 + 1*s1                                     = 1
  * 2*x1 + 2*x2 + 2*x3        + 1*s2                      - 1*a3  = -2
  * 1*x1 + 2*x2 + 3*x3               + 1*s3                       = 2 
  *
  * constructed tableau
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

  /* create tableau */
  matrix* tableau = get_zero_matrix(prob->inequality_count + prob->equality_count + 1, prob->variable_count*2 + prob->inequality_count + virtual_vars->count + 1); /* +1 rows for objective function, +1 columns for rhs */  

  /* intial basis */
  work_set* basis = work_set_create(prob->equality_count+prob->inequality_count);
  /* insert virtual vars first */
  for (int i = 1; i <= prob->equality_count; i++) {
    work_set_append(basis, i+prob->variable_count*2+prob->inequality_count);
  }
  /* insert slack vars */
  for (int i = 1; i <= prob->inequality_count; i++) {
    work_set_append(basis, i+prob->variable_count*2);
  }


//is this correct use of insert_sub_matrix??
  /* insert equality constraints */
  if (prob->equality_count > 0) {
    insert_sub_matrix(1, prob->equality_count, 1, prob->variable_count*2, Et, tableau);
    insert_sub_matrix(1, prob->equality_count, tableau->columns, tableau->columns, prob->h, tableau);
  }

  /* insert inequality constraints */
  if (prob->inequality_count > 0) {
    insert_sub_matrix(prob->equality_count + 1, prob->equality_count + prob->inequality_count, 1, prob->variable_count*2, Ft, tableau);
    insert_sub_matrix(prob->equality_count + 1, prob->equality_count + prob->inequality_count, tableau->columns, tableau->columns, gr, tableau);
  }

  //print_matrix(tableau);


  /* insert virtual variables */
  /*for (int r = 1; r <= prob->equality_count; r++) {
    insert_value_without_check(1, r, prob->variable_count + prob->inequality_count + r, tableau);
  }*/
  //for (int r = prob->equality_count+1; r < tableau->rows; r++) {

  //printf("eqc: %d\n", prob->equality_count);
  //printf("virvar: %d\n", virtual_vars->count);

  for (int r = 1; r <= virtual_vars->count; r++) {
    //printf("row: %d\n", r);
    if (virtual_vars->data[r-1] > prob->equality_count) {
      insert_value_without_check(-1, virtual_vars->data[r-1], prob->variable_count*2 + prob->inequality_count + r, tableau);
    } else {
      insert_value_without_check(1, virtual_vars->data[r-1], prob->variable_count*2 + prob->inequality_count + r, tableau);
    }  
  }

  /* insert slack variables */
  for (int r = prob->equality_count+1; r < tableau->rows; r++) {
    insert_value_without_check(1, r, prob->variable_count*2 + (r-prob->equality_count), tableau);
  }

  //print_matrix(tableau);

  /* insert objective function: min sum of virtual variables */
  for (int c = prob->variable_count*2 + prob->inequality_count + 1; c < tableau->columns; c++) {
    insert_value_without_check(-1, tableau->rows, c, tableau);
  }

  //print_matrix(tableau);
  

  /* adjust objective so it does not contain any virtual variables */
  /* remove vars from equality constraints */
  for (int r = 1; r <= prob->equality_count; r++) {
    add_rows(r, tableau->rows, tableau);
  }
  
  /* remove vars from inequality constraints */
  //int temp = tableau->columns-1;
  int temp_col = prob->variable_count*2 + prob->inequality_count + prob->equality_count + 1;


  for (int r = prob->equality_count+1; r <= tableau->rows-1; r++) {
    if (temp_col >= tableau->columns) {
      break;
    } else if (compare_elements(get_value_without_check(r, temp_col, tableau), 0) == 0) {
      continue;
    }
    /* removes virtual variable from objective function */
    matrix* temp_row = get_row_vector_with_return(r, tableau);
    multiply_matrix_with_scalar(-1, temp_row);
    matrix* temp_obj = get_row_vector_with_return(tableau->rows, tableau);
    matrix* new_obj = add_matrices_with_return(temp_row, temp_obj);
    insert_row_vector(tableau->rows, new_obj, tableau);

    /* free help matrices */
    free_matrix(temp_row);
    free_matrix(temp_obj);
    free_matrix(new_obj);

    /* next var */
    temp_col++;
  }

  //print_matrix(tableau);

  int column, row;
  value cur, last;
  bool error = false;

  /* set z0 to 0 */
  for (int r = 1; r <= prob->variable_count; r++) {
    insert_value_without_check(0, r, 1, prob->z0);
  }

  //print_matrix(tableau);

  /* solve */ 
  while (!is_neg_tableau_row(tableau->rows, tableau)) {

    column = -1;
    row = -1;
    cur = 0;
    last = 0;

    /* find most positive element in objective */
    for (int c = 1; c <= tableau->columns-1; c++) {
      cur = get_value_without_check(tableau->rows, c, tableau);
      if (compare_elements(cur,last) == 1) {        
        column = c;
        last = cur;
      }
    }

    /* shall not be able to happen */
    if (column == -1) {
      error = true;
      break;
    }

    /* run min test on choosen column */
    row = min_test(column, tableau);

    /* infeasible problem */
    if (column == -1) {
      error = true;
      break;
    }   

    /* change basis */
    basis->data[row-1] = column;

    /* get a 1 at choosen position */
    value div = get_value_without_check(row, column, tableau);
    divide_row_with_scalar(div, row, tableau);

    /* eliminate other values in the column */
    value scal;
    matrix* pivot_row;
    matrix* old_row;
    matrix* new_row;

    for (int r = 1; r <= tableau->rows; r++) {
      if (r == row) {
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

    //print_matrix(tableau);
    //work_set_print(basis);

  }
  
  //work_set_print(basis);

  value val;
  if (!error) {
    /* loop through basis to set variables */
    for (int r = 1; r <= basis->count; r++) {
      if (basis->data[r-1] <= prob->variable_count) {
        val = get_value_without_check(basis->data[r-1], 1, prob->z0) + get_value_without_check(r, tableau->columns, tableau);
        insert_value_without_check(val, basis->data[r-1], 1, prob->z0);
      } else if (basis->data[r-1] > prob->variable_count && basis->data[r-1] <= prob->variable_count*2) {
        val = get_value_without_check(basis->data[r-1]-prob->variable_count, 1, prob->z0) - get_value_without_check(r, tableau->columns, tableau);
        insert_value_without_check(val, basis->data[r-1]-prob->variable_count, 1, prob->z0);
      }
    }
  }

  //print_matrix(prob->z0);

  return !error;
}


