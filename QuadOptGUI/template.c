#include <matLib.h>
#include <solver.h>
#include <stdio.h>

int main(){
  /* Matrices Q and q should be created and initialized */
  matrix* Q = create_matrix(a, b);
  value Q_data[nr_of_elements] = {elements};
  insert_array(Q_data, Q);
  
  matrix* q = create_matrix(c, d);
  value q_data[nr_of_elements] = {elements};
  insert_array(q_data, q);

  /* For every summation value, their should be a matrix.
     Create and initalize the matrices like done above.
     Insert these matrices into the arrays shown below.
     
     E stands for equality constraints.
     h is a matrix for the right-hand side of the equality constraints.

     F is a matrix of inequality constraints.
     g is a matrix for the right-hand side of the inequality constraints.
     
     There can be n amount of matrices. 
  */
  matrix* E1;
  matrix* h1;
  matrix* EN;
  matrix* hN;

  matrix* F1;
  matrix* g1;
  matrix* FN;
  matrix* gN;

  /* For summation sign */
  int upper_bound = value;
  int lower_bound = value;
  int bound = upper_bound - lower_bound;
  float summation_value;

  matrix* equality_constraints[bound] = {E1,..,EN};
  matrix* equality_constraints_rhs[bound] = {h1,..,hN};
  matrix* inequality_constraints[bound] = {F1,..,FN};
  matrix* inequality_constraints_rhs[bound] = {g1,..,gN};

  matrix* x_start = NULL;
  matrix* u_start = NULL;
  
  int i;
  for(i = 0; i <= bound; i++){
    /* Constraints for the right index value */
    matrix* E = equality_constraints[i];
    matrix* h = equality_constraints_rhs[i];
    matrix* F = inequality_constraints[i];
    matrix* g = inequality_constraints_rhs[i];

    /* Create and solve the problem */
    problem* x = create_problem(Q, q, E, h, F, g, x_start, 0, 0);    
    problem* u = create_problem(Q, q, E, h, F, g, u_start, 0, 0); 

    quadopt_solver(x);
    quadopt_solver(u);

    x_start = x->solution;
    u_start = u->solution;
    
    /* Summarize the solution values */
    summation_value += x->solution_value + u->solution_value;

    /* Print solution for the n:th problem */
    printf("Solution for n = %i\n", lower_bound++);
    printf("x solution:\n");
    print_matrix(x->solution);
    printf("u solution:\n");
    print_matrix(u->solution);
    printf("x optimum value = %f\n", x->solution_value);
    printf("u optimum value = %f\n", u->solution_value);
  }
  printf("Total optimum value = %f\n", summation_value);  

  /* Free everything */
  free_problem(x);
  free_problem(u);

  for(i = 0; i <= bound; i++){
    free_matrix(equality_constraints[i]);
    free_matrix(equality_constraints_rhs[i]);
    free_matrix(inequality_constraints[i]);
    free_matrix(inequality_constraints_rhs[i]);
  }

  return 0;
}
