bool insert_value(value insert,int row, int col, matrix* mat) {
  if (!check_boundaries(row, col, mat)) {
    return false;
  }
  value* start = mat->start + mat->columns * (row - 1)  + (col - 1);
  *(start) = insert;
  return true;
}
