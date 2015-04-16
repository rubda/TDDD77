#include <matLib.h>
#include "work_set.h"

struct problem
{
	/* Optimization function */
	matrix* Q;
	matrix* Q_inv;

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

typedef struct problem problem;

problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, matrix* F, matrix* g, matrix* z0);

void print_problem(problem* prob);

void free_problem(problem* prob);

matrix* get_active_conditions(problem* prob);

void solve_subproblem(problem* prob);

matrix* quadopt_solver(problem* prob);

bool is_feasible_point(matrix* z, problem* prob);

void comb(int pool, int need, int* rows, int at, int ri, problem* prob, matrix* A, matrix* b, matrix* z, bool* done);

bool find_starting_point(problem* prob);

bool get_solution_value(problem* prob);

bool remove_constraint(problem* prob);
