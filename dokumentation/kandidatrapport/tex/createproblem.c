/** Puts matrices to a problem struct */
problem* create_problem(matrix* Q, matrix* q, matrix* E, matrix* h, 
			matrix* F, matrix* g, matrix* z0, int max_iter, 
			int max_micro_sec);
