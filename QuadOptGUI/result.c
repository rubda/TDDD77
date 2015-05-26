#include <matLib.h>
#include <solver.h>
#include <trans_con.h>
#include <assert.h>

int main(){

size_t N = 30;
size_t card_x = 2;
size_t card_u = 1;

size_t n_vars = card_x*(N+1) + card_u*N;

/* Variables */
matrix* z = create_matrix(n_vars, 1);

/* Coeffients */
matrix* Q = create_matrix(2,2);
value Q_data[4] = {4.0000,0,0,0.1000};
insert_array(Q_data, Q);

matrix* k = create_matrix(2,1);
value k_data[2] = {15,45};
insert_array(k_data, k);

matrix* P = create_matrix(2,2);
value P_data[4] = {59.2896,6.9782,6.9782,1.8280};
insert_array(P_data, P);

matrix* x_lim = create_matrix(4,4);
value x_lim_data[16] = {20,100,15,100};
insert_array(x_lim_data, x_lim);

matrix* Fx = create_matrix(12,2);
value Fx_data[24] = {-0.8037,-0.0461,-0.8442,-0.0404,-0.8845,-0.0332,0.9243,0.0243,-0.9243,-0.0243,0.9630,0.0133,-0.9630,-0.0133,1.0000,0,0,1.0000,-1.0000,0,0,-1.0000,0.6398,0.1555};
insert_array(Fx_data, Fx);

matrix* B = create_matrix(2,1);
value B_data[2] = {0.0143,0.6860};
insert_array(B_data, B);

matrix* A = create_matrix(2,2);
value A_data[4] = {0.9721,0.0155,0.2114,0.9705};
insert_array(A_data, A);

matrix* gx = create_matrix(12,1);
value gx_data[12] = {15,15,15,20,15,20,15,20,100,15,100,25};
insert_array(gx_data, gx);

matrix* u_lim = create_matrix(2,2);
value u_lim_data[4] = {25,25};
insert_array(u_lim_data, u_lim);

matrix* R = create_matrix(1,1);
value R_data[1] = {8};
insert_array(R_data, R);

/* Transform matrix stuff */
matrix* E = create_zero_matrix(card_x*(N + 1), n_vars);
matrix* h = create_zero_matrix(card_x*(N + 1), 1);
assert(trans_dyn_cons(A, B, k, E, h, card_x, card_u));

size_t rows = 2*card_x*N + F->rows + 2*card_u*N;
size_t cols = card_x*(N + 1) + card_u*N;
matrix* F = create_zero_matrix(rows, cols);
matrix* g = create_zero_matrix(rows, 1);
assert(trans_ineq_cons(Fx, gx, F, g, card_x, card_u, N, x_lim, u_lim));

multiply_matrix_with_scalar(-1, F);
multiply_matrix_with_scalar(-1, g);

matrix* Qfinal = create_zero_matrix(n_vars, n_vars);
create_objective(N, Q, P, R, Qfinal)

/* Solveranrop */ 
int i; 
for(i = 0; i <= 10; i++){problem* problem = create_problem(Qfinal, q, E, h, F, g, NULL, 0, 0);
quadopt_solver(problem);
print_solution(problem);
}

free_matrix(x_min);
free_matrix(Q);
free_matrix(k);
free_matrix(u_min);
free_matrix(P);
free_matrix(x_lim);
free_matrix(Fx);
free_matrix(B);
free_matrix(A);
free_matrix(x_max);
free_matrix(u_max);
free_matrix(gx);
free_matrix(u_lim);
free_matrix(R);

}