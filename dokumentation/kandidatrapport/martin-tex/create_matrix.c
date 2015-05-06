matrix* create_matrix(int row, int col) {
  if (row < 1 || col < 1) {
    return NULL;
  }

  matrix* mal = (matrix *) malloc(sizeof(matrix));
  mal->columns = col;
  mal->rows = row;
  mal->size = row * col;
  mal->start = (value *) malloc(col * row * sizeof(value));
  return mal;
}
