#include <matLib.h>
#include <assert.h>

int main(){
  matrix* m = create_matrix(2, 2);

  free_matrix(m);

  return 0;
}
