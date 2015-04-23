#include <matLib.h>
#include <solver.h>

int main(){
  /* For every summation value, their should be a matrix.
     Insert these matrices into the ararys shown below */
  matrix* E1;
  matrix* h1_rhs;
  matrix* EN;
  matrix* hN_rhs;

  matrix* F1;
  matrix* g1_rhs;
  matrix* FN;
  matrix* gN_rhs;

  /* For summation sign */
  int upper_bound = value;
  int lower_bound = value;
  float summation_value;

  matrix* equality_constraints[upper_bound] = {E1,..,EN};
  matrix* equality_constraints_rhs[upper_bound] = {h1,..,hN};
  matrix* inequality_constraints[upper_bound] = {F1,..,FN};
  matrix* inequality_constraints_rhs[upper_bound] = {g1,..,gN};

  matrix* x_start = NULL;
  matrix* u_start = NULL;
  
  int i;
  for(i = lower_bound; i <= upper_bound; i++){
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

    printf("Iteration %i\n");
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

  for(i = lower_bound; i <= upper_bound; i++){
    free_matrix(equality_constraints[i]);
    free_matrix(equality_constraints_rhs[i]);
    free_matrix(inequality_constraints[i]);
    free_matrix(inequality_constraints_rhs[i]);
  }
}
