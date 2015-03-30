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
  return prob;
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
}

void find_start_point(problem* prob){
  value temp_points[2]={2,0};
  value temp[prob->number_of_variables];
  for (int i =0;i<2;i++){
    temp[i]=temp_points[i];
  }
  prob->current_point=temp;
  prob->point_set=true;
}
