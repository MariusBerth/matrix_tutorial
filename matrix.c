#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "matrix.h"

#define PRINT_PRECISION "6"
#define PRINT_DECIMAL_PRECISION "2"

matrix matrix_create(unsigned n1, unsigned n2, scalar v)
{
  matrix m={n1,n2,true,NULL};
  if(n1==0 || n2==0)
    return m;
  else if(SIZE_MAX / n1 <= n2)
    m.ok = false;
  else if (!(m.data=calloc(((size_t) n1)*n2, sizeof(scalar))))
    m.ok = false;
  else {
    for(unsigned i=0; i<n1; ++i)
      for(unsigned j=0; j<n2; ++j)
        *matrix_get(m,i,j) = v;
  }
 
  return m;
}

matrix matrix_identity(unsigned n)
{
  matrix m = matrix_create(n, n, 0.);
  if(!m.ok)
    return m;

  for(unsigned i=0; i<n; ++i)
    *matrix_get(m, i, i) = 1.;
  return m;
}

void matrix_destroy(matrix m)
{
  if(m.ok) {
    m.ok = false;
    free(m.data);
  }
}

scalar *matrix_get(matrix m, unsigned i, unsigned j)
{
  if(!m.ok || i>m.n1 || i>m.n2)
    return NULL;

  return &m.data[i*m.n2+j];
}

matrix matrix_add(matrix m, matrix n)
{
  matrix res={0,0,false,NULL};

  if(m.n1!=n.n1 || m.n2!=n.n2 || !m.ok || !n.ok)
    return res;

  res=matrix_create(m.n1, m.n2, 0.);
  for(unsigned i=0; i<m.n1; ++i)
    for(unsigned j=0; j<m.n2; ++j)
      *matrix_get(res, i, j) = *matrix_get(m, i, j) + *matrix_get(n, i, j);

  return res;
}

void matrix_print(FILE *f, matrix m)
{
  if(!m.ok)
    fprintf(f, "Invalid matrix\n");
  else {
    for(unsigned i=0; i<m.n1; ++i) {
      for(unsigned j=0; j<m.n2; ++j)
        fprintf(
            f, 
            "%"PRINT_PRECISION"."PRINT_DECIMAL_PRECISION"f ",
            *matrix_get(m, i, j));
      fprintf(f, "\n");
   }
  }
}

matrix matrix_scal_mult(scalar lambda, matrix m) {
  matrix res = {0,0,false,NULL};
  if (!m.ok){
    return res;
  }
  else{
    int i,j;
    res = matrix_create(m.n1, m.n2, 0.);

    for(i=0; i<m.n1; i++){
      for(j=0; j<m.n2; j++){
        *matrix_get(res, i, j) = lambda * (*matrix_get(m, i, j));
      }
    }
    return res;
  }
}

matrix matrix_exp (matrix m, unsigned int n){
  matrix res ={0,0,false,NULL};
  if (!m.ok || m.n1 != m.n2){
    return res;
  }
  else{
    int ex = n;
    res = matrix_create(m.n1, m.n2, 0.);
    matrix base = m;
      
    while (ex >0){
      if ((ex % 2) == 1){
        matrix resaux = matrix_add (res, base);
        matrix_destroy(res);
        res = resaux;
        matrix_destroy(resaux);
      }
      matrix baseaux = matrix_mult(base,base);
      matrix_destroy (base);
      base = baseaux;
      matrix_destroy(baseaux);
      ex = ex/2;
    }
    return res;
  }
}

matrix matrix_mult(matrix a, matrix b)
{
  matrix res = {0, 0, false, NULL};

  if (!a.ok || !b.ok || a.n2 != b.n1)
    return res;
  
  res = matrix_create(a.n1, b.n2, 0);

  for (unsigned i = 0; i < a.n1; i++)
  {
    for (unsigned j = 0; j < b.n2; j++)
    {
      scalar *cell = matrix_get(res, i, j);
      
      for (unsigned k = 0; k < a.n2; k++)
      {
        *cell += *matrix_get(a, i, k) * *matrix_get(b, k, j);
      }
    }
  }

  return res;
}

scalar matrix_trace(matrix m) {
  if (m.n1==m.n2) {
    double res = 0.;
    for (unsigned int i = 0; i<m.n1; i++) {
      res += *matrix_get(m, i, i);
    }
    return res;
  }
  exit(53);
}

matrix tensor_product(matrix m, matrix n) {
  matrix res = matrix_create(m.n1*n.n1, m.n2*n.n2, 0.);
  for (unsigned int i = 0; i<m.n1*n.n1; i++) {
    for (unsigned int j = 0; j<m.n2*n.n2; j++) {
      *matrix_get(res, i, j) = (*matrix_get(m, i/n.n1, j/n.n2)) * (*matrix_get(n, i%n.n1, j%n.n2));
    }
  }
  return res;
}
