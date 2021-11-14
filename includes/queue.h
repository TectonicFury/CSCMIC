#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#define QUEUE(TYPE) \
typedef struct TYPE ## _queue * TYPE ## _queue; \
typedef struct TYPE ## _node * TYPE ## _node; \
struct TYPE ## _queue { \
  TYPE ## _node first; \
  TYPE ## _node last; \
}; \
struct TYPE ## _node { \
  TYPE val; \
  TYPE ## _node next; \
}; \
\
void init_ ## TYPE ## _queue(TYPE ## _queue *p_queue) { \
  *p_queue  = malloc(sizeof(struct TYPE ## _queue)); \
  (*p_queue)->first = NULL; \
  (*p_queue)->last = NULL; \
} \
void push_ ## TYPE ## _queue(TYPE ## _queue p_queue, TYPE val) { \
  if (!p_queue->first) { \
    p_queue->first = malloc(sizeof(struct TYPE ## _node)); \
    p_queue->first->val = val; \
    p_queue->first->next = NULL; \
    p_queue->last = p_queue->first; \
    p_queue->last->next = NULL; \
  } else { \
    TYPE ## _node temp = p_queue->last; \
    temp->next = malloc(sizeof(struct TYPE ## _node)); \
    temp->next->val = val; \
    temp->next->next = NULL; \
    p_queue->last = temp->next; \
  } \
} \
/* the caller has to explicitly free the dequeued node */ \
TYPE ## _node dequeue_ ## TYPE ## _queue(TYPE ## _queue p_queue) { \
  if (!p_queue->first) return NULL; \
  TYPE ## _node temp_node = p_queue->first; \
  if (p_queue->first ==  p_queue->last) { \
    p_queue->first = NULL; \
    p_queue->last = NULL; \
    return temp_node; \
  } \
  p_queue->first = p_queue->first->next; \
  return temp_node; \
} \
int is_empty_ ## TYPE ## _queue(TYPE ## _queue p_queue) { \
  return p_queue->first == NULL; \
}
#endif
