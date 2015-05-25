#ifndef TRANS_CON_H
#define TRANS_CON_H

/*
  Author: Alexander Yngve
  Email: aleyn573@student.liu.se
  Date: 2015-04-29
  Description: This file containts the necessary functions to transform dynamic
               constraints to normal optimization constraints.
*/

#include <matLib.h>
#include <stdbool.h>

/** Dynamic constraints (A and B with initial values K) transforms to equality constraints (E and h).*/
bool trans_dyn_cons(matrix* A, matrix *B, matrix* k, matrix* E, matrix* h, size_t card_x, size_t card_u);
bool trans_ineq_cons(matrix* Fx, matrix* gx, matrix* F, matrix* g, size_t card_x, size_t card_u, size_t N, matrix* x_lim, matrix* u_lim);
bool create_objective(int n, matrix* Qin, matrix* P, matrix* R, matrix* Q);

#endif /* TRANS_CON_H */
