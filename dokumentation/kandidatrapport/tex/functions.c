/** Create a matrix */
matrix* create_matrix(int row, int col);

/** Destroy a matrix */
void free_matrix(matrix* mat);

/** Prints the matrix*/
void print_matrix(matrix* mat);

/** Insert a value into matrix */
bool insert_value(value insert,int row, int col, matrix* mat);

/** Get a value from matrix */
value get_value(int row, int col, matrix* mat);

/** Adds a and b into c */
bool add_matrices(matrix* a, matrix* b, matrix* c);

/** Subtract a and b into c. c=a-b */
bool subtract_matrices(matrix* a, matrix* b, matrix* c);

/** Multiply a and b into c. c=a*b */
bool multiply_matrices(matrix* a, matrix* b, matrix* c);

/** Solves Ax=B */
bool solve_linear(matrix* a,matrix* x, matrix *b);
