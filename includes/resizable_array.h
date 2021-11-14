#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H
#include <stdlib.h>
#define ARRAY(TYPE) \
typedef struct TYPE ## _array * TYPE ## _array; \
\
struct TYPE ## _array { \
  TYPE *arr; \
  int size; \
  int len; \
  double load_factor; \
}; \
void init_ ## TYPE ## _array(TYPE ## _array *a) { \
  TYPE ## _array p = malloc(sizeof(struct TYPE ## _array)); \
  p->len = 8; \
  p->arr = malloc(8 * sizeof(TYPE)); \
  p->size = 0; \
  p->load_factor = 0.75; \
  *a = p; \
} \
void add_ ## TYPE ## _array(TYPE ## _array a, TYPE val) { \
  if (a->size >= a->load_factor * a->len) { \
    TYPE *p_arr = malloc(2 * a->len * sizeof(TYPE)); \
    for (int i = 0; i < a->size; i++) { \
      p_arr[i] = a->arr[i]; \
    } \
    free(a->arr); \
    a->arr = p_arr; \
    a->len = 2 * a->len; \
  } \
  a->arr[a->size] = val; \
  a->size++; \
}
#endif
