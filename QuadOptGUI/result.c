#include <matLib.h>
#include <solver.h>

int main(){

  /* Parameters */

  matrix* Q; 
  Q = create_matrix(4, 4);
  matrix* q; 
  q = create_matrix(1, 4);
  matrix* A; 
  A = create_matrix(4, 3);
  matrix* b; 
  b = create_matrix(1, 3);
  matrix* F; 
  F = create_matrix(4, 20);
  matrix* g; 
  g = create_matrix(1, 20);

  /* Variables */

  matrix* z; 
  z = create_matrix(4, 1);



  /* Insert values into matrices */

  value Q_data[16] = {10,0,0,0,0,2,0,0,0,0,5,0,0,0,0,100};
  insert_array(Q_data, Q);
  value q_data[4] = {0.5751,0.7510,0.1535,0.3568};
  insert_array(q_data, q);
  value A_data[12] = {-0.9163,-0.1967,-0.0123,0.6752,1.7876,-0.8901,0.0728,0.7860,-0.8204,0.9107,0.9394,-2.1327};
  insert_array(A_data, A);
  value b_data[3] = {0.2199,0.9663,0.3500};
  insert_array(b_data, b);
  value F_data[80] = {0.9018,0.0317,-1.0579,-0.6312,2.1212,0.3413,-0.6434,-0.5003,1.2486,-0.0835,0.2584,-0.8672,-1.1232,0.6164,0.8917,-1.0401,-0.8301,-0.5250,-0.8366,1.2654,-0.1218,1.0077,0.5531,-0.2415,-0.6426,1.8291,1.4584,-1.7290,-0.0789,0.0853,-0.8551,-0.4882,1.2281,-0.0683,-0.9921,1.0604,-0.5314,-0.5635,-0.0117,-0.5382,-0.2861,-0.4736,0.6269,1.7773,-0.2276,-1.7035,0.0015,-0.7795,0.6745,-0.0537,-0.8163,-0.7530,1.0368,-0.8813,0.1151,-1.0331,-0.1495,1.2559,0.2071,1.1638,-0.3171,0.1558,-0.4446,-0.5801,0.9336,0.0549,-1.1205,0.4173,0.7233,1.3986,0.4354,-1.6481,0.4882,-1.7756,0.0171,-0.5727,3.2662,0.3297,-0.3630,0.6592};
  insert_array(F_data, F);


  /* Solveranropp */ 

  int i; 
  for(i = 0; i <= 10; i++){ 
    problem* problem = create_problem(Q ,q ,A ,b ,F ,g ,0,1);
    quadopt_solver(problem);
    print_solution(problem);
 } 
}