#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#define List(KEY) \
typedef struct List_ ## KEY *List_ ## KEY; \
\
struct List_ ## KEY { \
  KEY key; \
  List_ ## KEY next; \
}; \
List_ ## KEY make_ ## KEY ## _list(KEY key) { \
  List_ ## KEY p = malloc(sizeof(struct List_ ## KEY)); \
  p->key = key; \
  p->next = NULL; \
  return p; \
}
#endif
