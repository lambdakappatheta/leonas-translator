#include <stdio.h>
#include <stdlib.h>

void f(){
  int* a = malloc(1024 * sizeof *a);
  a[0] = 1;
  float* b = malloc(1024 * sizeof *b);
  b[0] = 1.1;
  a = realloc(a, 512 * sizeof *a);
  b = realloc(b, 512 * sizeof *b);
  
}

int main(int argc, char const *argv[]) {
  
  f();
  
  int* x = malloc(sizeof *x);
  *x = 0;
  int* y = malloc(sizeof *y);
  *y = 1;
  
  int z = *x + *y;
  fprintf(stderr, "%d\n", z);
  
  return 0;
}
