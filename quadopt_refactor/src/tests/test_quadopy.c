#include <quadopt.h>

int main(){
matrix* G=create_matrix(2,2);
value temp_G[4]={2,0,0,2};
insert_array(temp_G,G);

matrix* d=create_matrix(2,1);
value temp_d[2]={-2,-5};
insert_array(temp_d,d);

matrix* A=create_matrix(5,2);
value temp_A[10]={1,-2,-1,-2,-1,2,1,0,0,1};
insert_array(temp_A,A);

matrix* b=create_matrix(5,1);
value temp_b[5]={-2,-6,-2,0,0};
insert_array(temp_b,b);
problem* main_problem= create_problem(G,d,A,b);
solve_problem(main_problem);
return 0;
}
