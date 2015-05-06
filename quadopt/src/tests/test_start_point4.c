#include <solver.h>
#include <assert.h>
#include <feasible_point.h>
#include <simplex.h>

int main() {

  /* Quadratic terms. */
  matrix* Q = create_matrix(4,4);
  value Q_arr[16] = {10, 0, 0, 0,
         0,  2, 0, 0,
         0,  0, 5, 0,
         0,  0, 0, 100};
  insert_array(Q_arr, Q);


  /* Linear terms. */
  matrix* q = create_matrix(4,1);
  value q_arr[4] = {0.5751,
        0.7510,
        0.1535,
        0.3568};
  insert_array(q_arr, q);


  /* Equality constraints LHS. */
  matrix* E = create_matrix(3, 4);
  value E_arr[12] = {-0.9163,   -0.1967,   -0.0123,    0.6752,
         1.7876,   -0.8901,    0.0728,    0.7860,
         -0.8204,    0.9107,    0.9394,   -2.1327};
  insert_array(E_arr, E);


  /* Equality constraints RHS. */
  matrix* h = create_matrix(3, 1);
  value h_arr[3] = {0.2199,
        0.9663,
        0.3500};
  insert_array(h_arr, h);

  /* Inequality constraints LHS. */
  matrix* F = create_matrix(20, 4);
  value F_arr[80] = {0.9018,    0.0317,   -1.0579,   -0.6312,
         2.1212,    0.3413,   -0.6434,   -0.5003,
         1.2486,   -0.0835,    0.2584,   -0.8672,
         -1.1232,    0.6164,    0.8917,   -1.0401,
         -0.8301,   -0.5250,   -0.8366,    1.2654,
         -0.1218,    1.0077,    0.5531,   -0.2415,
         -0.6426,    1.8291,    1.4584,   -1.7290,
         -0.0789,    0.0853,   -0.8551,   -0.4882,
         1.2281,   -0.0683,   -0.9921,    1.0604,
         -0.5314,   -0.5635,   -0.0117,   -0.5382,
         -0.2861,   -0.4736,    0.6269,    1.7773,
         -0.2276,   -1.7035,    0.0015,   -0.7795,
         0.6745,   -0.0537,   -0.8163,   -0.7530,
         1.0368,   -0.8813,    0.1151,   -1.0331,
         -0.1495,    1.2559,    0.2071,    1.1638,
         -0.3171,    0.1558,   -0.4446,   -0.5801,
         0.9336,    0.0549,   -1.1205,    0.4173,
         0.7233,    1.3986,    0.4354,   -1.6481,
         0.4882,   -1.7756,    0.0171,   -0.5727,
         3.2662,    0.3297,   -0.3630,    0.6592};
  insert_array(F_arr, F);
  multiply_matrix_with_scalar(-1, F);

  /* Inequality constraints RHS. */
  matrix* g = create_matrix(20, 1);
  value g_arr[20] = {2.5608,
         0.1002,
         1.0968,
         3.5569,
         1.2822,
         1.8934,
         0.7085,
         1.1576,
         0.1129,
         1.7866,
         2.2442,
         1.5517,
         1.9250,
         2.5216,
         0.2158,
         1.1543,
         1.0513,
         2.3039,
         1.7168,
         1.8912};
  insert_array(g_arr, g);
  multiply_matrix_with_scalar(-1, g);


  problem* problem = create_problem(Q, q, E, h, F, g, NULL, 0, 0);


  assert(simplex_phase_1(problem));
  assert(is_feasible_point(problem->z0, problem));

  //print_matrix(problem->z0);

  free_problem(problem);
}
