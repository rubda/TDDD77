#include "matLib.h"
#include "work_set.h"

struct qp_problem
{
	/* Optimization function */
	matrix* Q;
	matrix* Qn;

	matrix* q;

	/* Equality constraints */
	int equality_count;
	matrix* E;
	matrix* h;

	/* larger-than constraints */
	int inequality_count;
	matrix* F;
	matrix* g;

	/* All constraints */
	matrix* A;
	matrix* b;
	int constraints_count;

	/* variables */
	bool has_start_point;
	matrix* z0;

	matrix* z;

	matrix* solution;
	value solution_value;
	bool has_solution;

	matrix* p;
	value step;
	matrix* lagrange;

	work_set* active_set;
	work_set* sub_set;

	value accuracy;
};

typedef struct qp_problem qp_problem;

qp_problem* create_problem(matrix* Q, matrix* q, matrix* A, matrix* b, matrix* F, matrix* g, matrix* z0);

void print_qp_problem(qp_problem* prob);

/*calculate_lagrange();*/

/*value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws);

void get_unsolved(matrix* Ain, work_set* unsolved);
bool get_p(matrix* Ain, matrix* G, matrix* gk, matrix* d, matrix* z, matrix* p, matrix* lagrange, work_set* ws);

matrix* quadopt_solver(matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy);
*/

matrix* quadopt_solver1(qp_problem* prob);
