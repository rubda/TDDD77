#include "matLib.h"
#include "work_set.h"

struct qp_problem
{
	/* min(z) z^T*G*z + d*z
	 * s.t.
	 * A*z >= b 
	 */

	/* parameters */
	matrix* G;
	matrix* d;
	matrix* A;
	matrix* b;

	/* variables */
	matrix* z;
	matrix* p;
	matrix* lagrange;
};

typedef struct qp_problem qp_problem;

/*calculate_lagrange();*/

value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws);

void get_unsolved(matrix* Ain, work_set* unsolved);
bool get_p(matrix* Ain, matrix* G, matrix* gk, matrix* d, matrix* z, matrix* p, matrix* lagrange, work_set* ws);

matrix* quadopt_solver(matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy);
