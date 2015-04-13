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
	matrix* gk;
	value step;
	matrix* lagrange;

	work_set* active_set;

	value accuracy;
};

typedef struct qp_problem qp_problem;

qp_problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, matrix* F, matrix* g, matrix* z0);

void print_qp_problem(qp_problem* prob);

void free_qp_problem(qp_problem* prob);

matrix* get_active_conditions(qp_problem* prob);

void solve_subproblem(qp_problem* prob);

matrix* quadopt_solver(qp_problem* prob);
