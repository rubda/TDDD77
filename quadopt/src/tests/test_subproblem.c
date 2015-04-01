#include <assert.h>
#include <subproblem.h>
#include <matLib.h>
#include <work_set.h>

int main(){
  /* Creates A matrix */
  matrix* A = create_matrix(5, 2);
  value temp_a[10] = { 1, -2,
		      -1, -2,
		      -1,  2,
		       1,  0,
		       0,  1};
  insert_array(temp_a, A);

  /* Create work set */
  work_set* ws = work_set_create(2);
  work_set_append(ws, 3);
  //work_set_append(ws, 5);

  solve_supbroblem(A, NULL, NULL, NULL, NULL, NULL, NULL, ws);

  free_matrix(A);
  work_set_free(ws);
  return 0;
}
