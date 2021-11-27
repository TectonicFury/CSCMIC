#ifndef STACK_H
#include <stdlib.h>
#define STACK_H
#define STACK(TYPE) \
typedef struct TYPE ## _stack * TYPE ## _stack; \
struct TYPE ## _stack { \
  TYPE val; \
  TYPE ## _stack next; \
}; \
/* must call below function */ \
void init_ ## TYPE ## _stack(TYPE ## _stack *pp_stack) { \
  *pp_stack = NULL; \
} \
void push_ ## TYPE ## _stack(TYPE ## _stack *pp_stack, TYPE val) { \
  TYPE ## _stack p_push_stack = malloc(sizeof(struct TYPE ## _stack)); \
  p_push_stack->val = val; \
  p_push_stack->next = *pp_stack; \
  *pp_stack = p_push_stack; \
} \
/* the caller has to explicitly free the popped node */ \
TYPE ## _stack pop_ ## TYPE ## _stack(TYPE ## _stack *pp_stack) { \
  if (!(*pp_stack)) return NULL; \
  TYPE ## _stack p = *pp_stack; /* the value to be popped */ \
  *pp_stack = (*pp_stack)->next; \
  return p; \
} \
void aux_free_ ## TYPE ## _stack(TYPE ## _stack *p_stack) { \
  if (!(*p_stack)) return; \
  aux_free_ ## TYPE ## _stack(&((*p_stack)->next)); \
  free(*p_stack); \
  *p_stack = NULL; \
} \
void free_ ## TYPE ## _stack(TYPE ## _stack *pp_stack) { \
  aux_free_ ## TYPE ## _stack(pp_stack); \
  *pp_stack = NULL; \
}
#endif
