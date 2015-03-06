#include <matLib.h>
#include <work_set.h>
#include <stdbool.h>
#include <assert.h>

/* Finds the lagrange multipliers and removes constraints */
bool find_lagrange(matrix* Q, matrix* A, matrix* d, matrix* z, work_set* w){
	
  /* Calculates g = Gz + d */
  matrix* g = create_matrix(Q->rows, 1); 
  multiply_matrices(Q, z, g);
  add_matrices(g, d, g);

  /* Puts all the related conditions to w_mat depending on the work_set */
  matrix* tmp_row = create_matrix(1, w->count);
  matrix* w_mat = create_matrix(w->count, A->columns); 
  
  for (int i = 0; i < w->count; i++){
    assert(get_row_vector(w->data[i], A, tmp_row));
    //print_matrix(tmp_row);
    //assert(insert_row_vector(i, tmp_row, w_mat));
  }

  //Hårdkod just nu, då insert_row_vector verkar knasa för mig
  value temp_w[4] = {-1, 2, 0, 1};
  matrix* w_mat_step = create_matrix(w->count, A->columns); 
  insert_array(temp_w, w_mat_step);
 
  transpose_matrix(w_mat_step, w_mat);
  
  /* Solves the system W_mat * x = g */
  matrix* solved = create_matrix(w_mat->rows, 1);
  solve_linear(w_mat, solved, g);

  /* Finds the lowest lagrange value */
  int min_row = 1;
  value min_value = get_value(1, 1, solved);

  for (int i = 2; i <= w->count; i++){
    value temp = get_value(i, 1, solved);
    if (temp < min_value){
      min_value = temp;
      min_row = i;
    }
  }

  /* Removes the lowest lagrange value from the work set */
  work_set_remove(w, w->data[min_row]);

  /* Success */
  return true;
}
