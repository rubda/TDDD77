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

/* Presents the problem */
void present_problem(problem* prob){
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
  gauss_jordan(prob->A);

  matrix* sub_b=create_matrix(prob->number_of_active_conditions,1);
  matrix* temp1;
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
  problem* sub=prob->subproblem;
  matrix* x=solve_linear_with_return(sub->A,sub->b);
  if(x!=NULL){
    if (sub->point_set){
      free_matrix(sub->x);
    }
  sub->x=x;
  sub->point_set=true;
  }
  if (sub->number_of_conditions>sub->number_of_variables){
    /* handle to many equations */
  }
  if (sub->number_of_conditions<sub->number_of_variables){
    /* handle to few equations */
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