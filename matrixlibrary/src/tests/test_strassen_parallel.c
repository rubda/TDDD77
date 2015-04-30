#include <matLib.h>
#include <assert.h>


int main(){
#ifdef PARALLEL
  initialize_parallelization();

  double time_spent;
  int n=4096;
  matrix* a = create_identity_matrix(n,n);
  matrix* b=  create_identity_matrix(n,n);
  begin = clock();
  matrix* temp=strassen_matrices_parallel_with_return(a,b);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" %f \n",time_spent);
  return 0;

  deinitialize_parallelization();
#endif
  return 0;
}
