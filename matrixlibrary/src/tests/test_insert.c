#include <matLib.h>
#include <assert.h>

void test_insert_array(){
  matrix* m = create_matrix(2, 2);
  value m_values[4] = {2, 1, 7, 8};

  assert(insert_array(m_values, m));
  assert(m->start[0] == 2);
  assert(m->start[1] == 1);
  assert(m->start[2] == 7);
  assert(m->start[3] == 8);

  free_matrix(m);
}

void test_insert_value(){
  matrix* m = create_matrix(2, 2);
  value m_values[4] = {2, 5, 7, 8};

  assert(insert_array(m_values, m));

  assert(insert_value(1, 2, 1, m));
  assert(m->start[0] == 2);
  assert(m->start[1] == 5);
  assert(m->start[2] == 1);
  assert(m->start[3] == 8);

  free_matrix(m);
}

void test_insert_value_without_check(){
  matrix* m = create_matrix(2, 2);
  value m_values[4] = {2, 5, 7, 8};

  assert(insert_array(m_values, m));

  insert_value_without_check(1, 2, 1, m);
  assert(m->start[0] == 2);
  assert(m->start[1] == 5);
  assert(m->start[2] == 1);
  assert(m->start[3] == 8);

  free_matrix(m);
}

int main(){
  test_insert_array();
  test_insert_value();
  test_insert_value_without_check();

  return 0;
}
