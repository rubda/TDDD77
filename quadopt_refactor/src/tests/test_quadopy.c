#include <quadopt.h>

void test_random_data(){
  matrix* G=create_matrix(3,3);
  value temp_G[9]={1 ,1,1,1,0,1,1,1,0};
  insert_array(temp_G,G);

  matrix* d=create_matrix(3,1);
  value temp_d[3]={-2,-5,-3};
  insert_array(temp_d,d);

  matrix* A=create_matrix(7,3);
  value temp_A[21]={1,2,3,4,5,6,1,2,1,4,3,1,1,0,0,0,1,0,0,0,1};
  insert_array(temp_A,A);

  matrix* b=create_matrix(7,1);
  value temp_b[7]={20,1,2,3,0,0,0};
  insert_array(temp_b,b);
  problem* main_problem= create_problem(G,d,A,b);

  main_problem->x=create_matrix(3,1);
  insert_value(1,1,1,main_problem->x);
  insert_value(0,2,1,main_problem->x);
  insert_value(0,3,1,main_problem->x);
  main_problem->point_set=true;
  main_problem->max_iterations=2;
  solve_problem(main_problem);
}
void test_book_example(){
  matrix* G=create_matrix(2,2);
  value temp_G[4]={2,0,
                   0,2};
  insert_array(temp_G,G);

  matrix* d=create_matrix(2,1);
  value temp_d[2]={ -2,
                    -5};
  insert_array(temp_d,d);

  matrix* A=create_matrix(5,2);
  value temp_A[10]={  1,-2,
                      -1,-2,
                      -1,2,
                      1,0,
                      0,1};
  insert_array(temp_A,A);

  matrix* b=create_matrix(5,1);
  value temp_b[7]={ -2,
                    -6,
                    -2,
                    0,
                    0};
  insert_array(temp_b,b);
  problem* main_problem= create_problem(G,d,A,b);

  main_problem->x=create_matrix(2,1);
  insert_value(2,1,1,main_problem->x);
  insert_value(0,2,1,main_problem->x);
  main_problem->point_set=true;
  main_problem->max_iterations=2;
  solve_problem(main_problem);

}

int main(){
test_book_example();
return 0;
}
