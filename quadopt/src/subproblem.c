#include <subproblem.h>
#include <matLib.h>
#include <solver.h>
#include <assert.h>

void range_space_sparse(matrix* A, problem* prob){

  sparse_matrix* s_A = create_sparse_matrix(A, -1);
  matrix* At = transpose_matrix_with_return(A);  

  matrix* QAt = multiply_sparse_matrix_matrix(prob->sparse_Q_inv, At);
  matrix* AQAt = multiply_sparse_matrix_matrix(s_A, QAt);
  sparse_matrix* s_AQAt = create_sparse_matrix(AQAt, -1);

  matrix* Qg = multiply_sparse_matrix_matrix(prob->sparse_Q_inv, prob->gk);
  matrix* AQg = multiply_sparse_matrix_matrix(s_A, Qg);
  
  matrix* Az = multiply_sparse_matrix_matrix(s_A, prob->z);


  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  matrix* h1 = subtract_matrices_with_return(AQg, c);  

  matrix* lambda = create_zero_matrix(AQg->rows, AQg->columns);   

  /* solve to retrieve lambda */
  conjugate_gradient(s_AQAt, lambda, h1);
  //gauss_jordan_solver(AQAt, lambda, h1);  

  matrix* ht = multiply_matrices_with_return(At, lambda);
  matrix* h2 = subtract_matrices_with_return(ht, prob->gk);

  /* solve to retrieve p */
  conjugate_gradient(prob->sparse_Q, prob->p, h2);
  

  matrix* Qp = multiply_sparse_matrix_matrix(prob->sparse_Q, prob->p);
  
  if(compare_matrices(Qp, prob->gk)){
    int i;
    for(i = 1; i <= prob->p->rows; i++){
      insert_value_without_check(0, i, 1, prob->p);
    }
  }

  free_sparse_matrix(s_A);
  free_sparse_matrix(s_AQAt);
  free_matrix(QAt);
  free_matrix(At);
  free_matrix(Qg);
  free_matrix(AQAt);
  free_matrix(AQg);
  free_matrix(Az);
  free_matrix(h1);
  free_matrix(lambda);
  free_matrix(ht);
  free_matrix(h2);
  free_matrix(Qp);
  free_matrix(b);
  free_matrix(c);
}

void range_space(matrix* A, problem* prob){


  matrix* At = transpose_matrix_with_return(A);  

  matrix* AQ = multiply_matrices_with_return(A, prob->Q_inv);

  matrix* AQAt = multiply_matrices_with_return(AQ, At);

  matrix* AQg = multiply_matrices_with_return(AQ, prob->gk);
  
  matrix* Az = multiply_matrices_with_return(A, prob->z);

  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  matrix* h1 = subtract_matrices_with_return(AQg, c);  

  matrix* lambda = create_matrix(AQg->rows, AQg->columns);
  gauss_jordan_solver(AQAt, lambda, h1);  

  matrix* ht = multiply_matrices_with_return(At, lambda);
  matrix* h2 = subtract_matrices_with_return(ht, prob->gk);

  gauss_jordan_solver(prob->Q, prob->p, h2);  

  matrix* Qp = multiply_matrices_with_return(prob->Q, prob->p);
  
  if(compare_matrices(Qp, prob->gk)){
    int i;
    for(i = 1; i <= prob->p->rows; i++){
      insert_value_without_check(0, i, 1, prob->p);
    }
  }

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
  free_matrix(b);
  free_matrix(c);
}


void KKT_sub_sparse(matrix* A, problem* prob){

  sparse_matrix* s_A = create_sparse_matrix(A, -1);

  /* create sparse verison of lhs */
  int n = matrix_sparsity(A);
  sparse_matrix* s_K = create_empty_sparse_matrix(n + n + prob->sparse_Q->size);
  int i;

  int dest = 0;
  /* copy Q */
  memcpy(s_K->A, prob->sparse_Q->A, prob->sparse_Q->size*sizeof(value));
  memcpy(s_K->rA, prob->sparse_Q->rA, prob->sparse_Q->size*sizeof(int));
  memcpy(s_K->cA, prob->sparse_Q->cA, prob->sparse_Q->size*sizeof(int));
  dest += prob->sparse_Q->size;

  /* copy A */
  memcpy(s_K->A + dest, s_A->A, s_A->size*sizeof(value));
  memcpy(s_K->cA + dest, s_A->cA, s_A->size*sizeof(int));
  for (i = 0; i < s_A->size; i++) {
    s_K->rA[dest+i] = s_A->rA[i] + prob->Q->rows;
  }
  dest += s_A->size;

  /* copy At */
  transpose_sparse_matrix(s_A);
  memcpy(s_K->A + dest, s_A->A, s_A->size*sizeof(value));
  memcpy(s_K->rA + dest, s_A->rA, s_A->size*sizeof(int));
  for (i = 0; i < s_A->size; i++) {
    s_K->cA[dest+i] = s_A->cA[i] + prob->Q->columns;
  }

  /* transpose back */
  transpose_sparse_matrix(s_A);

  s_K->rows = prob->Q->rows + A->rows;
  s_K->columns = prob->Q->columns + A->rows; /* transpose columns */

  /* Vars */
  matrix* pl = create_zero_matrix(prob->variable_count+A->rows, 1);

  /* Rhs */
  matrix* gc = create_matrix(pl->rows,1);

  /* Fill rhs */
  matrix* Az = multiply_sparse_matrix_matrix(s_A, prob->z);

  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  insert_sub_matrix(1, prob->gk->rows, 1, 1, prob->gk, gc);
  insert_sub_matrix(prob->gk->rows+1, gc->rows, 1, 1, c, gc);


  /* Solve kkt system */
  conjugate_gradient(s_K, pl, gc);

  /* Retrieve p */
  for (i = 1; i <= prob->p->rows; i++){
    insert_value_without_check(-get_value_without_check(i, 1, pl), i, 1, prob->p);
  }
   
  free_matrix(pl);
  free_matrix(gc);
  free_matrix(Az);
  free_matrix(b);
  free_matrix(c);
  free_sparse_matrix(s_K);
  free_sparse_matrix(s_A);
}

void KKT_sub(matrix* A, problem* prob){

  matrix* At = transpose_matrix_with_return(A);

  /* Lhs */
  matrix* K = create_zero_matrix(prob->Q->rows+A->rows, prob->Q->columns+At->columns);

  /* Fill lhs */
  insert_sub_matrix(1, prob->Q->rows, 1, prob->Q->columns, prob->Q, K);
  insert_sub_matrix(prob->Q->rows+1, K->rows, 1, A->columns, A, K);
  insert_sub_matrix(1, At->rows, prob->Q->columns+1, K->columns, At, K);

  /* Vars */
  matrix* pl = create_matrix(prob->variable_count+A->rows, 1);

  /* Rhs */
  matrix* gc = create_matrix(pl->rows,1);

  /* Fill rhs */
  matrix* Az = create_matrix(A->rows ,prob->z->columns);
  multiply_matrices(A, prob->z, Az);  

  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  insert_sub_matrix(1, prob->gk->rows, 1, 1, prob->gk, gc);
  insert_sub_matrix(prob->gk->rows+1, gc->rows, 1, 1, c, gc);

  /* Solve kkt system */
  gauss_jordan_solver(K, pl, gc);
  //conjugate_gradient(K, pl, gc);

  /* Retrieve p */
  int i;
  for (i = 1; i <= prob->p->rows; i++){
    insert_value_without_check(-get_value_without_check(i, 1, pl), i, 1, prob->p);
  }
   
  free_matrix(At);
  free_matrix(K);
  free_matrix(pl);
  free_matrix(gc);
  free_matrix(Az);
  free_matrix(b);
  free_matrix(c);
}


/* Solves the subproblem for active set */
void solve_subproblem(problem* prob){
 /* gk */
  matrix* tmp = create_matrix(prob->q->rows, 1);
  multiply_matrices(prob->Q, prob->z, tmp);
  add_matrices(tmp, prob->q, prob->gk);
  free_matrix(tmp);

  if (prob->active_set->count == 0){
    /* Solve derivative and get vector pointing towards the global minimum */
    value sum, d_val;

    int r, c;
    for (c = 1; c <= prob->Q->columns; c++){
      sum = 0;
      for (r = 1; r <= prob->Q->rows; r++) {
        sum += get_value_without_check(r, c, prob->Q);
      }
      d_val = get_value_without_check(c, 1, prob->gk);
      insert_value_without_check((-d_val)/sum, c, 1, prob->p);
    }
    return;
  }

  /* Solve system as long as you get the the zero vector */
  matrix* A = get_active_conditions(prob);

  bool success;
  do{
    success = ((A->rows >= A->columns)); //|| gauss_jordan(A));

    if(success){
      /* Remove condition */
      if (!remove_constraint(prob)){

	int i;
        for(i = 1; i <= prob->p->rows; i++){
          insert_value_without_check(0, i, 1, prob->p);
        }
        free_matrix(A);
        return;
      }
      
      /* Resize A matrix */
      free_matrix(A);
      A = get_active_conditions(prob);
    }
  } while(success);

  /* Use range-space to get p */

  if (prob->is_sparse) {
    /* range_space_sparse(A, prob); */
    range_space(A, prob);
    /* KKT_sub_sparse(A, prob); */

  } else {
    range_space(A, prob);
    /* KKT_sub(A, prob); */
  }
 
  free_matrix(A);

}
