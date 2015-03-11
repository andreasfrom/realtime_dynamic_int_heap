#include <stdlib.h>
#include <stdio.h>
#include "realtime_dynamic_int_heap.h"

int main() {
  size_t n;
  scanf("%lu\n", &n);

  int * xs = malloc(n * sizeof *xs);
  for (size_t i = 0; i < n; i++)
    scanf("%d", &xs[i]);

  heap_sort(xs, n);

  for (size_t i = 0; i < n; i++)
    printf("%d ", xs[i]);
  
  return 0;
}
