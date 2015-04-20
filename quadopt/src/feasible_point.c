#include "feasible_point.h"

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

/* Iterate through all possible combinations of inequality constraints to add */
void comb(int pool, int need, int* rows, int at, int ri, problem* prob, 
	  matrix* A, matrix* b, matrix* z, bool* done){
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
  if (prob->equality_count > 0 && prob->inequality_count > 0){    

    /* Variables */
    matrix* A = create_matrix(prob->z->rows, prob->z->rows);
    matrix* b = create_matrix(prob->z->rows, 1);
    matrix* tmp_A;
    value tmp_b;

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

    comb(pool, need, rows, 0, need, prob, A, b, prob->z0, &done);

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
