#ifndef FIND_LAGRANGE_H
#define FIND_LAGRANGE_H

#include <matLib.h>
#include "work_set.h"

bool find_lagrange(matrix* g, matrix* A, matrix* d, matrix* z, work_set* w, matrix* lagrange);

#endif /* FIND_LAGRANGE_H */
