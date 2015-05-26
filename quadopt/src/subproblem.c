#include <subproblem.h>
#include <matLib.h>
#include <solver.h>
#include <assert.h>

void range_space_sparse(sparse_matrix* A, problem* prob){

  transpose_sparse_matrix(A);
  matrix* At = sparse_to_normal(A);
  transpose_sparse_matrix(A); 

  matrix* QAt = multiply_sparse_matrix_matrix(prob->sparse_Q_inv, At);
  matrix* AQAt = multiply_sparse_matrix_matrix(A, QAt);
  sparse_matrix* s_AQAt = create_sparse_matrix(AQAt, -1);
  matrix* Qg = multiply_sparse_matrix_matrix(prob->sparse_Q_inv, prob->gk);
  matrix* AQg = multiply_sparse_matrix_matrix(A, Qg);
  matrix* Az = multiply_sparse_matrix_matrix(A, prob->z);
  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);
  matrix* h1 = subtract_matrices_with_return(AQg, c);  

  prob->lagrange = create_zero_matrix(AQg->rows, AQg->columns);   

  /* Solve to retrieve lagrange multiplicators */
  //conjugate_gradient(s_AQAt, prob->lagrange, h1);
  gauss_jordan_solver(AQAt, prob->lagrange, h1);

  transpose_sparse_matrix(A);
  matrix* ht = multiply_sparse_matrix_matrix(A, prob->lagrange);
  matrix* h2 = subtract_matrices_with_return(ht, prob->gk);

  /* Solve to retrieve p */
  //conjugate_gradient(prob->sparse_Q, prob->p, h2);
  gauss_jordan_solver(prob->Q, prob->p, h2);
  
  matrix* Qp = multiply_sparse_matrix_matrix(prob->sparse_Q, prob->p);
  
  if(compare_matrices(Qp, prob->gk)){
    size_t i;
    for(i = 1; i <= prob->p->rows; i++){
      insert_value_without_check(0, i, 1, prob->p);
    }
  }

  free_sparse_matrix(s_AQAt);
  free_matrix(QAt);
  free_matrix(At);
  free_matrix(Qg);
  free_matrix(AQAt);
  free_matrix(AQg);
  free_matrix(Az);
  free_matrix(h1);
  free_matrix(prob->lagrange);
  prob->lagrange = NULL;
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
  prob->lagrange = create_matrix(AQg->rows, AQg->columns);
  gauss_jordan_solver(AQAt, prob->lagrange, h1);

  matrix* ht = multiply_matrices_with_return(At, prob->lagrange);
  matrix* h2 = subtract_matrices_with_return(ht, prob->gk);

  gauss_jordan_solver(prob->Q, prob->p, h2);  

  matrix* Qp = multiply_matrices_with_return(prob->Q, prob->p);
  
  if(compare_matrices(Qp, prob->gk)){
    size_t i;
    for(i = 1; i <= prob->p->rows; i++){
      insert_value_without_check(0, i, 1, prob->p);
    }
  }

  free_matrix(At);
  free_matrix(AQ);
  free_matrix(AQAt);
  free_matrix(AQg);
  free_matrix(Az);
  free_matrix(b);
  free_matrix(c);
  free_matrix(h1);
  free_matrix(prob->lagrange);
  prob->lagrange = NULL;
  free_matrix(ht);
  free_matrix(h2);
  free_matrix(Qp);
}


void KKT_sub_sparse(sparse_matrix* A, problem* prob){

  sparse_matrix* s_K = create_empty_sparse_matrix(A->size + A->size + prob->sparse_Q->size);
  size_t i;

  int dest = 0;
  /* Copy Q */
  memcpy(s_K->A, prob->sparse_Q->A, prob->sparse_Q->size*sizeof(value));
  memcpy(s_K->rA, prob->sparse_Q->rA, prob->sparse_Q->size*sizeof(int));
  memcpy(s_K->cA, prob->sparse_Q->cA, prob->sparse_Q->size*sizeof(int));
  dest += prob->sparse_Q->size;

  /* Copy A */
  memcpy(s_K->A + dest, A->A, A->size*sizeof(value));
  memcpy(s_K->cA + dest, A->cA, A->size*sizeof(int));
  for (i = 0; i < A->size; i++){
    s_K->rA[dest+i] = A->rA[i] + prob->Q->rows;
  }
  dest += A->size;

  /* Copy At */
  transpose_sparse_matrix(A);
  memcpy(s_K->A + dest, A->A, A->size*sizeof(value));
  memcpy(s_K->rA + dest, A->rA, A->size*sizeof(int));
  for (i = 0; i < A->size; i++){
    s_K->cA[dest+i] = A->cA[i] + prob->Q->columns;
  }

  /* Transpose back */
  transpose_sparse_matrix(A);

  s_K->rows = prob->Q->rows + A->rows;
  /* Transpose columns */
  s_K->columns = prob->Q->columns + A->rows; 

  /* Vars */
  matrix* pl = create_zero_matrix(prob->variable_count+A->rows, 1);

  /* RHS */
  matrix* gc = create_matrix(pl->rows,1);

  /* Fill RHS */
  matrix* Az = multiply_sparse_matrix_matrix(A, prob->z);

  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  insert_sub_matrix(1, prob->gk->rows, 1, 1, prob->gk, gc);
  insert_sub_matrix(prob->gk->rows+1, gc->rows, 1, 1, c, gc);


  /* Solve KKT system */
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
}

void KKT_sub(matrix* A, problem* prob){

  matrix* At = transpose_matrix_with_return(A);

  /* LHS */
  matrix* K = create_zero_matrix(prob->Q->rows+A->rows, prob->Q->columns+At->columns);

  /* Fill LHS */
  insert_sub_matrix(1, prob->Q->rows, 1, prob->Q->columns, prob->Q, K);
  insert_sub_matrix(prob->Q->rows+1, K->rows, 1, A->columns, A, K);
  insert_sub_matrix(1, At->rows, prob->Q->columns+1, K->columns, At, K);

  /* Vars */
  matrix* pl = create_matrix(prob->variable_count+A->rows, 1);

  /* RHS */
  matrix* gc = create_matrix(pl->rows,1);

  /* Fill RHS */
  matrix* Az = create_matrix(A->rows ,prob->z->columns);
  multiply_matrices(A, prob->z, Az);  

  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  insert_sub_matrix(1, prob->gk->rows, 1, 1, prob->gk, gc);
  insert_sub_matrix(prob->gk->rows+1, gc->rows, 1, 1, c, gc);

  /* Solve KKT system */
  gauss_jordan_solver(K, pl, gc);

  /* Retrieve p */
  size_t i;
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

    size_t r, c;
    for (c = 1; c <= prob->Q->columns; c++){
      sum = 0;
      for (r = 1; r <= prob->Q->rows; r++){
        sum += get_value_without_check(r, c, prob->Q);
      }
      d_val = get_value_without_check(c, 1, prob->gk);
      insert_value_without_check((-d_val)/sum, c, 1, prob->p);
    }
    return;
  }

  /* Solve system as long as we get the the zero vector */
  while (prob->active_set->count >= prob->variable_count){
    if (!remove_constraint(prob)){
        size_t i;
        for(i = 1; i <= prob->p->rows; i++){
          insert_value_without_check(0, i, 1, prob->p);
        }
        return;
      }
  }


  /* Use range-space to get p */
  if (prob->is_sparse){
    sparse_matrix* s_A = get_sparse_active_conditions(prob);
    range_space_sparse(s_A, prob);
    /* KKT_sub_sparse(s_A, prob); */
    free_sparse_matrix(s_A);
  }else{
    matrix* A = get_active_conditions(prob);
    range_space(A, prob);
    /* KKT_sub(A, prob); */
    free_matrix(A);
  }
}
