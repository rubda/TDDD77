#include <quadopt.h>

/* Create a problem */
problem* create_problem(matrix* G,matrix* d,matrix* A,matrix* b){
  if (G->rows!=G->columns||G->rows!=d->rows||d->columns!=1||A->columns!=G->rows||A->rows!=b->rows||b->columns!=1){
    return NULL;
  }
  problem* prob = (problem *) malloc(sizeof(problem));
  prob->G=G;
  prob->d=d;
  prob->A=A;
  prob->b=b;
  prob->number_of_conditions=A->rows;
  prob->number_of_variables=G->rows;
  prob->current_value=-1;
  prob->marginal=0;
  prob->active_conditions=create_matrix(1,1);
  insert_value(-1,1,1,prob->active_conditions);
  prob->subproblem_set=false;
  prob->point_set=false;
  prob->variable_dependencies_set=false;
  prob->iteration=0;
  return prob;
}

void free_problem(problem* prob){
  free_matrix(prob->G);
  free_matrix(prob->d);
  free_matrix(prob->A);
  free_matrix(prob->b);
  if (prob->point_set){
    free_matrix(prob->x);
  }
  if (prob->subproblem_set){
    free_problem(prob->subproblem);
  }

}

/* Sets the active_conditions in the problem struct */
void get_active_conditions(problem* prob){
  int active_conditions[prob->number_of_conditions];
  int number_of_active_conditions=0;
  for (int i=1;i<=prob->number_of_conditions;i++){
    matrix* ai=get_row_vector_with_return(i,prob->A);
    if(ai==NULL){
      continue;
    }
    matrix* aix=multiply_matrices_with_return(ai,prob->x);
    value temp=get_value(1,1,aix);
    value temp1=get_value(i,1,prob->b);
    if ((abs(temp-temp1)<=prob->marginal)||(abs(temp1-temp)<=prob->marginal)){
      active_conditions[number_of_active_conditions]=i;
      number_of_active_conditions++;
    }
    free_matrix(ai);
    free_matrix(aix);
  }
  matrix* conditions=create_matrix(1,number_of_active_conditions);
  for (int i=1;i<=number_of_active_conditions;i++){
    insert_value(active_conditions[i-1],1,i,conditions);
  }
  free_matrix(prob->active_conditions);
  prob->active_conditions=conditions;
  prob->number_of_active_conditions=number_of_active_conditions;
}


void generete_variable_dependencies_in_subproblem(problem* prob){
  problem* sub = prob->subproblem;
  /* we start by assuming that each variable has no relation to another variable */
  matrix* dependencies = create_identity_matrix(sub->number_of_variables,sub->number_of_variables);
  matrix* zero_vector = create_zero_matrix(1, sub->number_of_variables);
  matrix* sub_a = sub->A;
  int column;
  /* for each row in the subproblems A matrix*/
  for (int i = 1; i <= sub_a->rows; i++) {

    /* A row with only one variable, this means that it must be zero */
    if (first_nonezero_in_row_index(i, 1, sub_a) != 0|| first_nonezero_in_row_index(i + 1, 1, sub_a) == 0) {
      insert_row_vector(first_nonezero_in_row_index(i, 1, sub_a), zero_vector,dependencies);
    }
    /* A row with more than one variable, this means that they must be dependent */
    if (first_nonezero_in_row_index(i, 1, sub_a) != 0|| first_nonezero_in_row_index(i + 1, 1, sub_a) != 0) {
      column = first_nonezero_in_row_index(i, 1, sub_a);
      /* We zero this variable so it does not depend on itself */
      insert_value_without_check(0, column, column, dependencies);
      /* Copy values from sub_a to dependencies */
      for (int j = column + 1; j <= sub->number_of_variables; j++) {
        insert_value(-get_value(column, j, sub_a), column, j, dependencies);
      }
    }
  }
  free_matrix(zero_vector);
  sub->variable_dependencies=dependencies;
  sub->variable_dependencies_set=true;
}

/* Presents the problem */
void present_problem(problem* prob){
  printf("\n------------------------------------------------------------------------------\n");
  printf("the current iteration is: %i \n",prob->iteration);
  printf("The G matrix is:\n");
  print_matrix(prob->G);

  printf("The d matrix is:\n");
  print_matrix(prob->d);

  printf("The A matrix is:\n");
  print_matrix(prob->A);

  printf("The b matrix is:\n");
  print_matrix(prob->b);
  printf("The current value of the target function is: ");
  printf(FORMAT_STRING,prob->current_value);
  printf("\n");
  if(prob->point_set){
  printf("in the point: \n");
  print_matrix(prob->x);
  }
  printf("the active condition(s) are: \n");
  print_matrix(prob->active_conditions);
  if (prob->variable_dependencies_set){
    printf("the variable dependencies matris is: \n");
    print_matrix(prob->variable_dependencies);
  }
  if (prob->subproblem_set){
    printf("\n ---------------------------------------------------------------------------\n");
    printf("the current subproblem is: \n \n");
    present_problem(prob->subproblem);
  }
}

/* Solves the problem struct using active set method */
bool solve_problem(problem* prob){
  if (!prob->point_set){
    find_start_point(prob);
  }
  get_active_conditions(prob);
  create_subproblem(prob);
  solve_subproblem(prob);

#ifdef DEBUG
  calculate_current_value(prob);
  present_problem(prob);
#endif
  return true;
}

/* Creates a subproblem */
void create_subproblem(problem* prob){
  matrix* Gx=multiply_matrices_with_return(prob->G,prob->x);
  assert(Gx!=NULL);
  matrix* Gxplusd=add_matrices_with_return(Gx,prob->d);
  assert(Gxplusd!=NULL);
  free_matrix(Gx);
  matrix* sub_A=create_matrix(prob->number_of_active_conditions,prob->number_of_variables);
  matrix* temp;
  /* Create matrix with all active conditions */
  for (int i=1;i<=prob->number_of_active_conditions;i++){
    temp=get_row_vector_with_return(get_value(1,i,prob->active_conditions),prob->A);
    insert_row_vector(i,temp,sub_A);
    free_matrix(temp);
  }

  matrix* sub_b=create_matrix(prob->number_of_active_conditions,1);
  /* Create matrix with right hand side of all active conditions */
  for (int i=1;i<=prob->number_of_active_conditions;i++){
    insert_value(0,i,1,sub_b);
  }
  if (prob->subproblem_set){
    free_problem(prob->subproblem);
  }
  prob->subproblem=create_problem(prob->G,Gxplusd,sub_A,sub_b);
  prob->subproblem_set=true;

}

/* Solves the sub problem */
void solve_subproblem(problem* prob){
  problem* sub = prob->subproblem;

  /* Handle to many conditions */
  if (sub->number_of_conditions > sub->number_of_variables) {

  }
  /* Handle to few conditions */
  if (sub->number_of_conditions < sub->number_of_variables) {
    handle_to_few_conditions(prob);


  }

  /* Handle the same amounts of variables as equations */
  matrix* x = solve_linear_with_return(sub->A, sub->b);
  if (x != NULL) {
    if (sub->point_set) {
      free_matrix(sub->x);
    }
    sub->x = x;
    sub->point_set = true;
    return;
  }
  /* If solver fails  */
  else {
    free_matrix(x);
  }
}

void handle_to_many_conditions(problem* prob){
  problem* sub=prob->subproblem;
  matrix* temp = matrix_copy(sub->A);
  gauss_jordan(temp);
  matrix* A = get_matrix_with_only_pivots(temp);
  free_matrix(temp);
  if (A->columns == A->rows) {
    free_matrix(sub->b);
    sub->b = create_zero_matrix(A->rows, 1);
    free_matrix(sub->A);
    sub->A = A;
    if (sub->point_set) {
      free_matrix(sub->x);
    }
    sub->x = solve_linear_with_return(sub->A, sub->b);
    return;
  }
  else {
    free_matrix(sub->A);
    sub->A=A;
    handle_to_few_conditions(prob);
  }
}

void handle_to_few_conditions(problem* prob){
  problem* sub=prob->subproblem;
  generete_variable_dependencies_in_subproblem(prob);
  matrix* dep=sub->variable_dependencies;
  matrix* temp_vector;
  matrix* temp_vector_trans;
  matrix_m* p1=create_matrix_m(1,dep->rows);
  matrix_m* p2=create_matrix_m(dep->rows,1);
  matrix* temp;
  for (int i=1;i<=dep->rows;i++){
    temp_vector=get_row_vector_with_return(i,dep);
    temp_vector_trans=transpose_matrix_with_return(temp_vector);
    insert_matrix(1,i,temp_vector_trans,p1);
    temp=multiply_matrices_with_return(temp_vector_trans,temp_vector);
    free_matrix(temp_vector);
    free_matrix(temp_vector_trans);
  }
  for (int i=1;i<=dep->rows;i++){
    temp_vector=get_row_vector_with_return(i,dep);
    insert_matrix(i,1,temp_vector,p2);
    free_matrix(temp_vector);
  }
  matrix_m* sub_G_m=create_matrix_m_from_matrix(sub->G);

  matrix_m* p1G=multiply_matrices_m_with_return(p1,sub_G_m);

  matrix_m* p1Gp2=multiply_matrices_m_with_return(p1G,p2);

  print_matrix_m(p1Gp2);

  matrix* G_solv=get_matrix(1,1,p1Gp2);
  for (int i=1;i<=dep->rows;i++){
    print_matrix(derivate_matrix_with_return(i,G_solv));
  }


}

/* Calculates the functions current value */
bool calculate_current_value(problem* prob){
  matrix* x_t=transpose_matrix_with_return(prob->x);
  if (x_t==NULL){
    free_matrix(x_t);
    return false;
  }
  matrix* x_tG=multiply_matrices_with_return(x_t,prob->G);
  if (x_tG==NULL){
    free_matrix(x_t);
    free_matrix(x_tG);
    return false;
  }
  matrix* x_tGx=multiply_matrices_with_return(x_tG,prob->x);
  if (x_tGx==NULL){
    free_matrix(x_t);
    free_matrix(x_tG);
    free_matrix(x_tGx);
    return false;
  }
  multiply_matrix_with_scalar(0.5,x_tGx);
  matrix* d_t=transpose_matrix_with_return(prob->d);
  matrix* d_tx=multiply_matrices_with_return(d_t,prob->x);
  if (d_tx==NULL){
    free_matrix(x_t);
    free_matrix(x_tG);
    free_matrix(x_tGx);
    free_matrix(d_tx);
    return false;
  }
  matrix* min_x=add_matrices_with_return(x_tGx,d_tx);
  if (min_x==NULL){
    free_matrix(x_t);
    free_matrix(x_tG);
    free_matrix(x_tGx);
    free_matrix(d_tx);
    free_matrix(min_x);
    return false;
  }
  prob->current_value=get_value(1,1,min_x);
  free_matrix(x_t);
  free_matrix(x_tG);
  free_matrix(x_tGx);
  free_matrix(d_tx);
  free_matrix(min_x);
  return true;

}

/* This is just dummy for debugging, must be done correctly later */
void find_start_point(problem* prob){
  prob->x=create_matrix(2,1);
  insert_value(2,1,1,prob->x);
  insert_value(0,2,1,prob->x);
  prob->point_set=true;
}
