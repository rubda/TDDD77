#include <matLib.h>
#include <assert.h>
#include <time.h>
int main(){
#ifdef PARALLEL
  matrix* to_multiply;
  matrix* multiplied;
  clock_t begin, end;
  double time_spent;
  initialize_parallelization();
  /* int shit=4096; */
  /* int number_of_tests=shit; */
  /* for (int j=1;j<=1;j++){ */
  /* for (int i=shit;i<=number_of_tests;i*=2){ */
  /*   to_multiply=create_identity_matrix(i,i); */
  /*   multiplied=create_identity_matrix(i,i); */
  /*   assert(strassen_matrices_parallel(to_multiply,to_multiply,multiplied)); */
  /*   free_matrix(to_multiply); */
  /*   free_matrix(multiplied); */
  /* }} */
  deinitialize_parallelization();

  return 0;
#endif
}
