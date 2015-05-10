#include <subproblem.h>
#include <matLib.h>
#include <solver.h>





/* solves Ax = b, x should be set to 0 */
bool conjugate_gradient(matrix* A, matrix* x, matrix* b) {
  /* variables */
  value alpha, beta;

  matrix* nom = create_matrix(1, 1);
  matrix* Ap = create_matrix(A->rows, 1);
  matrix* r = matrix_copy(b);
  matrix* p = matrix_copy(r);    
  matrix* pt;

  /* solve */
  value rs_old = dot_product(r, r);
  value rs_new;

  while (true) {

    pt = transpose_matrix_with_return(p);

    /* calculate alpha */
    multiply_matrices(A, p, Ap);
    multiply_matrices(pt, Ap, nom);
    alpha = rs_old/get_value_without_check(1, 1, nom);

    /* calculate next x */
    multiply_matrix_with_scalar(alpha, p);
    add_matrices(x, p, x);

    /* calculate next r */
    multiply_matrix_with_scalar(alpha, Ap);
    subtract_matrices(r, Ap, r);

    rs_new = dot_product(r, r);

    /* check if approx. done */
    if (compare_elements(rs_new, 0) == 0) {
      break;
    }

    /* calculate beta */
    beta = rs_new/rs_old;

    /* calculate next p */
    multiply_matrix_with_scalar(beta, p);
    add_matrices(r, p, p);

    free_matrix(pt);

  }

  free_matrix(p);
  free_matrix(r);
  free_matrix(Ap);
  free_matrix(nom);

  return true;
}



void range_space(matrix* A, problem* prob){


  matrix* At = transpose_matrix_with_return(A);  

  matrix* AQ = create_matrix(A->rows, prob->Q_inv->columns);
  multiply_matrices(A,prob->Q_inv, AQ);  

  matrix* AQAt = create_matrix(AQ->rows, At->columns);
  multiply_matrices(AQ, At, AQAt);  

  matrix* AQg = create_matrix(AQ->rows, prob->gk->columns);
  multiply_matrices(AQ, prob->gk, AQg);  

  matrix* Az = create_matrix(A->rows ,prob->z->columns);
  multiply_matrices(A, prob->z, Az);  

  matrix* b = get_active_conditions_rhs(prob);
  matrix* c = subtract_matrices_with_return(Az, b);

  matrix* h1 = subtract_matrices_with_return(AQg, c);  

  matrix* lambda = create_matrix(AQg->rows, AQg->columns);
  gauss_jordan_solver(AQAt, lambda, h1);  

  matrix* ht = create_matrix(prob->p->rows, lambda->columns);
  matrix* h2 = create_matrix(ht->rows, ht->columns);
  multiply_matrices(At, lambda, ht);
  subtract_matrices(ht, prob->gk, h2);

  gauss_jordan_solver(prob->Q, prob->p, h2);
  //conjugate_gradient(prob->Q, prob->p, h2);

  matrix* Qp = create_matrix(prob->gk->rows, prob->gk->columns);
  multiply_matrices(prob->Q, prob->p, Qp);
  
  if(compare_matrices(Qp, prob->gk)){
    int i;
    for(i = 1; i <= prob->p->rows; i++){
      insert_value_without_check(0, i, 1, prob->p);
    }
  }

  free_matrix(A);
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

  /* Ska detta frigöras? */
   
  free_matrix(A);
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

  range_space(A, prob);

  /* KKT_sub(A, prob); */

}
