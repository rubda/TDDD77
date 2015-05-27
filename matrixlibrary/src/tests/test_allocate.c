#include <matLib.h>
#include <assert.h>
#include <time.h>
int main() {
  matrix* temp;
  clock_t begin, end;
  double time_spent;
  begin = clock();
  for (int j = 1; j <= 1000; j++) {
    temp=create_matrix(100,100);
    free_matrix(temp);
  }
  end = clock();
  time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
  printf("time taken was %f \n", time_spent);
  return 0;
}
