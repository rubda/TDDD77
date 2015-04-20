#include "subproblem.h"
#include <matLib.h>
#include "solver.h"

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
    success = gauss_jordan(A);

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
  solve_linear(AQAt, lambda, h1);  

  matrix* ht = create_matrix(prob->p->rows, lambda->columns);
  matrix* h2 = create_matrix(ht->rows, ht->columns);
  multiply_matrices(At, lambda, ht);
  subtract_matrices(ht, prob->gk, h2);

  solve_linear(prob->Q, prob->p, h2);

  #ifdef DEBUG

  printf("\n----- A -----\n");
  print_matrix(A);

  printf("\n----- At -----\n");
  print_matrix(At);

  printf("\n----- Q -----\n");
  print_matrix(prob->Q);

  printf("\n----- Q_inv -----\n");
  print_matrix(prob->Q_inv);

  printf("\n----- gk -----\n");
  print_matrix(prob->gk);

  printf("\n----- AQ -----\n");
  print_matrix(AQ);
  
  printf("\n----- AQAt -----\n");
  print_matrix(AQAt);

  printf("\n----- AQg -----\n");
  print_matrix(AQg);

  printf("\n----- Az -----\n");
  print_matrix(Az);

  printf("\n----- h1 -----\n");
  print_matrix(h1);

  printf("\n----- lambda -----\n");
  print_matrix(lambda);

  printf("\n----- h2 -----\n");
  print_matrix(h2);


  printf("\n------ p -----\n");
  print_matrix(prob->p);
  #endif


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
