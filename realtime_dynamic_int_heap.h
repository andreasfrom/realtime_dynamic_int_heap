#include <stdlib.h>
#include <stdbool.h>
#include "realtime_dynamic_int_stack/realtime_dynamic_int_stack.h"

typedef Stack Heap;

#define heap_alloc stack_alloc
#define heap_free stack_free
#define heap_size stack_size
#define heap_is_empty stack_is_empty

void heap_insert(Heap * const heap, int const x);
int heap_replace(Heap * const heap, int const x);
int heap_max(Heap const * const heap);
int heap_extract_max(Heap * const heap);
void heap_increase_key(Heap * const heap, size_t const ix, int const k);
void heap_delete(Heap * const heap, size_t const ix);

void heapify(int * const xs, size_t const n);
void heap_sort(int * const xs, size_t const n);

Heap * heap_from_array(int * const xs, size_t const n);
int * heap_degenerate_to_sorted_array(Heap * const heap, size_t * const size_out);
