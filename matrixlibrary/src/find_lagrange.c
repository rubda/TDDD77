#include "matLib.h"


/* Find langrange multiplier */


/* gk = Gxk + d */ 

/* (1/2) * z^T*Q*z + q*z 
 * Az => b + s
*/


bool gauss_elimination(matrix* a, matrix* b, matrix* c){
	/* Find the row with the largest value */
}

work_set get_g(matrix* Q, matrix* A, matrix* d, matrix* x0, work_set* W){
	matrix* g0;
 	multiply_matrices(G, x0, g0);
 	add_matrices(g0, d, g0);
} 