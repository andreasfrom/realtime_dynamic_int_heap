#include <stdlib.h>
#include <stdbool.h>
#include "realtime_dynamic_int_stack/realtime_dynamic_int_stack.h"

typedef Stack Heap;

Heap * heap_alloc(void);
void heap_free(Heap * const heap);
size_t heap_size(Heap const * const heap);
bool heap_is_empty(Heap const * const heap);

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
