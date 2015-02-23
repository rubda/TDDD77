#include <stdio.h>
#include <gsl/gsl_linalg.h>

void print_matrix(gsl_matrix* mat){
  int i;
  for (i = 0; i < mat->size1; i++){
    int j;
    for (j = 0; j < mat->size2; j++){
      printf("%f ", gsl_matrix_get(mat, i, j));
    }
    printf("\n");
  }
}

int main (void){
  double b_data[] = {1, 2, 3, 4};
  gsl_matrix_view b = gsl_matrix_view_array(b_data, 2, 2);
  print_matrix(&b.matrix);
  gsl_matrix_transpose(&b.matrix);
  print_matrix(&b.matrix);

  return 0;
}
