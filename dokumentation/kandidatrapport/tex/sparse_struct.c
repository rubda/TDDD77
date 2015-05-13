struct sparse_matrix {
  int size;
  int rows;
  int columns;
  value* A;
  int* rA;
  int* cA;
};