#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "realtime_dynamic_int_heap.h"

static inline size_t parent(size_t ix);
static inline size_t left(size_t ix);
static inline size_t right(size_t ix);

static void bubble_up(Heap * const heap, size_t const ix);
static void bubble_down(Heap * const heap, size_t const ix);

static void bubble_down_fixed(int * const xs, size_t const n, size_t const ix);
static int extract_max_fixed(int * const xs, size_t const n);
static void sort_heap_fixed(int * const xs, size_t const n);

static inline size_t parent(size_t ix) {
  return (ix-1) / 2;
}

static inline size_t left(size_t ix) {
  return ix * 2 + 1;
}

static inline size_t right(size_t ix) {
  return ix * 2 + 2;
}

static void bubble_up(Heap * const heap, size_t const ix) {
  assert(heap);
  assert(ix < heap_size(heap));

  if (ix == 0) return;

  if (stack_get(heap, parent(ix)) < stack_get(heap, ix)) {
    int const t = stack_get(heap, ix);
    stack_set(heap, ix, stack_get(heap, parent(ix)));
    stack_set(heap, parent(ix), t);
    bubble_up(heap, parent(ix));
  }
}

static void bubble_down(Heap * const heap, size_t const ix) {
  assert(heap);
  assert(ix < heap_size(heap));

  if (left(ix) >= stack_size(heap)) return;

  size_t largest_ix;

  if (right(ix) >= stack_size(heap) ||
      stack_get(heap, left(ix)) > stack_get(heap, right(ix)))
    largest_ix = left(ix);
  else
    largest_ix = right(ix);
  
  if (stack_get(heap, largest_ix) > stack_get(heap, ix)) {
    int const t = stack_get(heap, ix);
    stack_set(heap, ix, stack_get(heap, largest_ix));
    stack_set(heap, largest_ix, t);
    bubble_down(heap, largest_ix);
  }
}

static void bubble_down_fixed(int * const xs, size_t const n, size_t const ix) {
  assert(xs);

  if (left(ix) >= n) return;

  size_t largest_ix;

  if (right(ix) >= n || xs[left(ix)] > xs[right(ix)])
    largest_ix = left(ix);
  else
    largest_ix = right(ix);
  
  if (xs[largest_ix] > xs[ix]) {
    int const t = xs[ix];
    xs[ix] = xs[largest_ix];
    xs[largest_ix] = t;
    bubble_down_fixed(xs, n, largest_ix);
  }
}

void heap_insert(Heap * const heap, int const x) {
  assert(heap);

  stack_push(heap, x);
  bubble_up(heap, heap_size(heap)-1);
}

int heap_replace(Heap * const heap, int const x) {
  assert(heap);
  assert(heap_size(heap) > 0);

  int ret = stack_get(heap, 0);
  stack_set(heap, 0, x);
  bubble_down(heap, 0);

  return ret;
}

int heap_extract_max(Heap * const heap) {
  assert(heap);
  assert(!heap_is_empty(heap));

  if (heap_size(heap) == 1)
    return stack_pop(heap);
  
  int ret = stack_get(heap, 0);
  int last = stack_pop(heap);
  stack_set(heap, 0, last);
  bubble_down(heap, 0);    

  return ret;
}

static int extract_max_fixed(int * const xs, size_t const n) {
  assert(xs);
  assert(n > 0);

  int ret = xs[0];
  if (n == 1) return ret;
  xs[0] = xs[n-1];
  bubble_down_fixed(xs, n-1, 0);    

  return ret;
}

int heap_max(Heap const * const heap) {
  assert(heap);
  assert(!heap_is_empty(heap));
  return stack_get(heap, 0);
}


void heap_increase_key(Heap * const heap, size_t const ix, int const k) {
  assert(heap);
  assert(ix < heap_size(heap));
  assert(k >= 0);
  
  stack_set(heap, ix, stack_get(heap, ix) + k);
  bubble_up(heap, ix);
}

void heap_delete(Heap * const heap, size_t const ix) {
  assert(heap);
  assert(ix < heap_size(heap));

  if (ix == heap_size(heap)-1) {
    stack_pop(heap);
    return;
  }
  
  int const new = stack_pop(heap);
  int const old = stack_get(heap, ix);
  stack_set(heap, ix, new);
  if (new > old)
    bubble_up(heap, ix);
  else if (new < old)
    bubble_down(heap, ix);
}

void heapify(int * const xs, size_t const n) {
  assert(xs);
  for (int i = (int)n/2; i >= 0; i--)
    bubble_down_fixed(xs, n, (size_t)i);
}

void heap_sort(int * const xs, size_t const n) {
  assert(xs);
  heapify(xs, n);
  sort_heap_fixed(xs, n);
}

static void sort_heap_fixed(int * const xs, size_t const n) {
  for (size_t i = 0; i < n; i++) {
    int const max = extract_max_fixed(xs, n-i);
    xs[(n-1)-i] = max;
  }
}

Heap * heap_from_array(int * const xs, size_t const n) {
  assert(xs);
  heapify(xs, n);
  Heap * heap = heap_alloc();
  for (size_t i = 0; i < n; i++)
    stack_push(heap, xs[i]);
  return heap;
}

int * heap_degenerate_to_sorted_array(Heap * const heap, size_t * const size_out) {
  assert(heap);

  size_t n;
  int * xs = stack_degenerate_to_array(heap, &n);
  *size_out = n;
  sort_heap_fixed(xs, n);
  
  return xs;
}
