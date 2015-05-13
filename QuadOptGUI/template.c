#include <matLib.h>
#include <solver.h>
#include <trans_con.h>
#include <assert.h>

int main(){
  size_t card_x = 2;
  size_t card_u = 1;
  size_t N = 30;
  size_t n_vars = card_x*(N + 1) + card_u*N;

  /* Variables */
  matrix* z; 
  z = create_matrix(n_vars, 1);

  /* Coefficients */
  matrix* Q; 
  Q = create_matrix(2, 2);
  value Q_data[4] = {4, 0, 4, 0};
  insert_array(Q_data, Q);

  matrix* q;
  q = create_matrix(2, 1);
  value q_data[2] = {0, 0};
  insert_array(q_data, q);

  matrix* P;
  P = create_matrix(2, 2);
  value P_data[4] = {59.2896, 6.9782,
		     6.9782, 1.8280};
  insert_array(P_data, P);

  matrix* R;
  R = create_matrix(1, 1);
  value R_data[1] = {8};
  insert_array(R_data, R);


  /* Parameters */
  matrix* A;
  A = create_matrix(2, 2);
  value A_data[4] = {0.9721, 0.0155,
		     0.2114, 0.9705};
  insert_array(A_data, A);

  matrix* B;
  B = create_matrix(2, 1);
  value B_data[2] = {0.0143, 0.6860};

  matrix* Fx;
  Fx = create_matrix(12, 2);
  value Fx_data[24] = {-0.8037, -0.0461,
		      -0.8442, -0.0404,
		      -0.8845, -0.0332,
		      0.9243, 0.0243,
		      -0.9243, -0.0243,
		      0.9630, 0.0133,
		      -0.9630, -0.0133,
		      1.0000, 0,
		      0, 1.0000,
		      -1.0000, 0,
		      0, -1.0000,
		      0.6398, 0.1555};
  insert_array(Fx_data, Fx);

  matrix* gx;
  gx = create_matrix(12, 1);
  value gx_data[12] =    {15,
			 15,
			 15,
			 20,
			 15,
			 20,
			 15,
			 20,
			 100,
			 15,
			 100,
			 25};
  insert_array(gx_data, gx);

  matrix* Fu;
  Fu = create_matrix(1, 1);
  value Fu_data[1] = {0};
  insert_array(Fu_data, Fu);

  matrix* gu;
  gu = create_matrix(1, 1);
  value gu_data[1] = {0};
  insert_array(gu_data, gu);

  matrix* k;
  k = create_matrix(2, 1);
  value k_data[2] = {15, 45};
  insert_array(k_data, k);


  /* Transform matrix stuff */
  matrix* E = create_zero_matrix(card_x*(N + 1), n_vars);
  matrix* h = create_zero_matrix(card_x*(N + 1), 1);
  assert(trans_dyn_cons(A, B, k, E, h, card_x, card_u));
  print_matrix(E);
  print_matrix(h);
  
  matrix* F = create_zero_matrix(Fx->rows + Fu->rows, Fx->columns + Fu->columns);
  matrix* g = create_zero_matrix(gx->rows + gu->rows, 1);
  assert(trans_ineq_cons(Fx, Fu, gx, gu, F, g));
  print_matrix(F);
  print_matrix(g);

  /* Solveranropp */ 
  int i; 
  for(i = 0; i <= 10; i++){ 
    problem* problem = create_problem(Q, q, E, h, F, g, NULL, 0, 0);
    quadopt_solver(problem);
    print_solution(problem);
  }

  free_matrix(Q);
  free_matrix(q);
  free_matrix(P);
  free_matrix(R);
  free_matrix(A);
  free_matrix(B);
  free_matrix(Fx);
  free_matrix(gx);
  free_matrix(Fu);
  free_matrix(gu);
  free_matrix(k);
  free_matrix(E);
  free_matrix(h);
  free_matrix(F);
  free_matrix(g);
}
  
