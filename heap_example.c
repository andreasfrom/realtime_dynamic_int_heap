#include <stdlib.h>
#include <stdio.h>
#include "realtime_dynamic_int_heap.h"

static void heap_print(Heap const * const heap);

static void heap_print(Heap const * const heap) {
  for (size_t i = 0; i < heap_size(heap)-1; i++)
    printf("%d ", stack_get(heap, i));
  printf("%d\n", stack_get(heap, heap_size(heap)-1));
}

int main() {
  size_t n;
  scanf("%lu\n", &n);
  
  Heap * const heap = heap_alloc();

  char cmd[3];
  int x, y;

  for (size_t i = 0; i < n; i++) {
    cmd[1] = '\0';
    scanf("%s %d %d\n", cmd, &x, &y);

    switch (cmd[0]) {
    case 'I':
      if (cmd[1] == 'K')
	heap_increase_key(heap, (size_t)x, y);
      else
	heap_insert(heap, x);
      break;
    case 'E':
      printf("%d\n", heap_extract_max(heap));
      break;
    case 'D':
      heap_delete(heap, (size_t)x);
      break;
    case 'P':
      heap_print(heap);
      break;
    default:
      printf("Unknown command: %s\n", cmd);
      break;
    }
  }
  
  heap_free(heap);
  
  return 0;
}
