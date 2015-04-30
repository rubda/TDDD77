#include <matLib.h>
#include <assert.h>
#include <assert.h>
#include <time.h>
int main(){
  clock_t begin, end;
  double time_spent;
  int n=4096;
  matrix* a = create_identity_matrix(n,n);
  matrix* b=  create_identity_matrix(n,n);
  begin = clock();
  matrix* temp=strassen_matrices_with_return(a,b);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf(" %f \n",time_spent);
  return 0;
}
