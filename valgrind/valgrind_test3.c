#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  
  int* a = malloc(sizeof *a);
  int* elem = a + (1024 * 1024);
  
  free(a);
  
  int z = *elem;
  fprintf(stderr, "%d\n", z);
  
  return 0;
}
