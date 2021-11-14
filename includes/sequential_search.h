#ifndef SQS_H
#define SQS_H
#include <stdlib.h>
#define SequentialSearchST(KEY, VALUE, EQUALS) \
typedef struct KEY ## _ ## VALUE ## _listST * KEY ## _ ## VALUE ## _listST; \
typedef struct KEY ## _ ## VALUE ## _pairST * KEY ## _ ## VALUE ## _pairST; \
struct KEY ## _ ## VALUE ## _pairST { \
  /* basic pair structure to hold key and value and return from find() just like std::pair */ \
  KEY key; \
  VALUE value; \
}; \
struct KEY ## _ ## VALUE ## _listST { \
  KEY ## _ ## VALUE ## _pairST value_type; \
  KEY ## _ ## VALUE ## _listST next; \
}; \
KEY ## _ ## VALUE ## _listST make_ ## KEY ## _ ## VALUE ## _listST(KEY key, VALUE value) { \
  KEY ## _ ## VALUE ## _listST p_node = malloc(sizeof(struct KEY ## _ ## VALUE ## _listST)); \
  KEY ## _ ## VALUE ## _pairST p_value_type = malloc(sizeof(struct KEY ## _ ## VALUE ## _pairST)); \
  p_value_type->key = key; \
  p_value_type->value = value; \
  p_node->value_type = p_value_type; \
  p_node->next = NULL; \
  return p_node; \
} \
KEY ## _ ## VALUE ## _listST insert_ ## KEY ## _ ## VALUE ## _listST(KEY ## _ ## VALUE ## _listST first, KEY key, VALUE value) { \
  if (!first) { \
    return make_ ## KEY ## _ ## VALUE ## _listST(key, value); \
  } \
  if (EQUALS ## _(key, first->value_type->key)) { \
    first->value_type->value = value; \
    return first; \
  } \
  first->next = insert_ ## KEY ## _ ## VALUE ## _listST(first->next, key, value); \
  return first; \
} \
KEY ## _ ## VALUE ## _pairST find_in_ ## KEY ## _ ## VALUE ## _listST(KEY ## _ ## VALUE ## _listST first, KEY key) { \
  if (!first) return NULL; \
  if (EQUALS ## _(key, first->value_type->key)) return first->value_type; \
  return find_in_ ## KEY ## _ ## VALUE ## _listST(first->next, key); /* tail recursive ? */ \
} \
KEY ## _ ## VALUE ## _listST delete_ ## KEY ## _ ## VALUE ## _listST(KEY ## _ ## VALUE ## _listST first, KEY key, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  if (!first) return NULL; \
  if (EQUALS ## _(key, first->value_type->key)) { \
    KEY ## _ ## VALUE ## _listST temp_next = first->next; \
    destruct(first->value_type); \
    free(first); \
    return temp_next; \
  } \
  first->next = delete_ ## KEY ## _ ## VALUE ## _listST(first->next, key, destruct); \
  return first; \
} \
void free_whole_ ## KEY ## _ ## VALUE ## _listST(KEY ## _ ## VALUE ## _listST first, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  if (!first) return; \
  free_whole_ ## KEY ## _ ## VALUE ## _listST(first->next, destruct); \
  destruct(first->value_type); \
  free(first); \
 }
#endif
