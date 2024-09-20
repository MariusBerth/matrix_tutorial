#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdio.h>

typedef double scalar;

typedef struct matrix {
  unsigned n1, n2;
  bool ok;
  scalar *data;
} matrix;

matrix matrix_create(unsigned n1, unsigned n2, scalar v);
void matrix_destroy(matrix m);
matrix matrix_identity(unsigned n);
scalar *matrix_get(matrix m, unsigned i, unsigned j);
matrix matrix_add(matrix m, matrix n);
void matrix_print(FILE *f, matrix m);
matrix matrix_scal_mult(scalar lambda, matrix m);
matrix matrix_exp(matrix m, unsigned int n);

matrix add_matrix(matrix m, matrix n);
float matrix_trace(matrix m);

#endif /* MATRIX_H */
