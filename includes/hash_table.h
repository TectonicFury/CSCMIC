#ifndef HASHT_H
#define HASHT_H
#define M_LENGTH 97
#include "sequential_search.h"
#include "prime.h"

#define HashTable(KEY, VALUE, EQUALS, HASH_FUNC) \
SequentialSearchST(KEY, VALUE, EQUALS) /* defining the list for separate chaining */ \
typedef struct KEY ## _ ## VALUE ## _listST ** KEY ## _ ## VALUE ## _hash_table_array; \
typedef struct KEY ## _ ## VALUE ## _hash_table * KEY ## _ ## VALUE ## _hash_table; \
\
struct KEY ## _ ## VALUE ## _hash_table { \
  KEY ## _ ## VALUE ## _hash_table_array ht; \
  int size; \
  int capacity; \
  double load_factor; \
}; \
\
void initialise_ ## KEY ## _ ## VALUE ## _hash_table_array(KEY ## _ ## VALUE ## _hash_table_array *ht) { \
  *ht = malloc(M_LENGTH * sizeof(KEY ## _ ## VALUE ## _listST)); /* size of listST ptr is used as it is array of pointers to lists */ \
  for (size_t i = 0; i < M_LENGTH; i++) { \
    (*ht)[i] = NULL; /* this is a must otherwise segfault */\
  }\
} \
void init_ ## KEY ## _ ## VALUE ## _hash_table(KEY ## _ ## VALUE ## _hash_table *h_table) { \
  *h_table = malloc(sizeof(struct KEY ## _ ## VALUE ## _hash_table)); \
  (*h_table)->size = 0; \
  (*h_table)->load_factor = 0.75; \
  (*h_table)->capacity = M_LENGTH; \
  initialise_ ## KEY ## _ ## VALUE ## _hash_table_array(&((*h_table)->ht)); \
} \
void insert_ ## KEY ## _ ## VALUE ## _hash_table_array(KEY ## _ ## VALUE ## _hash_table_array *ht, KEY key, VALUE value, int modulo) { \
  unsigned int index = HASH_FUNC ## _(key, modulo);\
  (*ht)[index] = insert_ ## KEY ## _ ## VALUE ## _listST((*ht)[index], key, value); \
} \
\
void free_ ## KEY ## _ ## VALUE ## _hash_table_array(KEY ## _ ## VALUE ## _hash_table_array *ht, void (*destruct) (KEY ## _ ## VALUE ## _pairST), int capacity) { \
  for (int i = 0; i < capacity; i++) { \
    free_whole_ ## KEY ## _ ## VALUE ## _listST((*ht)[i], destruct); \
  } \
  free(*ht); \
  *ht = NULL; \
} \
\
/* resizing and rehashing the 'ht' array using a prime number greater than twice the current M */ \
void resize_and_rehash_ ## KEY ## _ ## VALUE ## _hash_table(KEY ## _ ## VALUE ## _hash_table h_table, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  unsigned int new_bucket_capacity = prime_twice(h_table->capacity); \
  KEY ## _ ## VALUE ## _hash_table_array ht_new = malloc(new_bucket_capacity * sizeof(KEY ## _ ## VALUE ## _listST)); \
  for (size_t i = 0; i < new_bucket_capacity; i++) { \
    ht_new[i] = NULL; /* this is a must otherwise segfault */\
  }\
  for (size_t i = 0; i < h_table->capacity; i++) { \
    KEY ## _ ## VALUE ## _listST ref_list = h_table->ht[i]; \
    for (; ref_list != NULL; ref_list = ref_list->next) { \
      insert_ ## KEY ## _ ## VALUE ## _hash_table_array(&ht_new, ref_list->value_type->key, ref_list->value_type->value, new_bucket_capacity); \
    }\
  }\
  free_ ## KEY ## _ ## VALUE ## _hash_table_array(&(h_table->ht), destruct, h_table->capacity); \
  h_table->ht = ht_new; \
  h_table->capacity = new_bucket_capacity; \
} \
\
KEY ## _ ## VALUE ## _pairST find_in_ ## KEY ## _ ## VALUE ## _hash_table_array(KEY ## _ ## VALUE ## _hash_table_array ht, KEY key, int modulo) { \
  int index = HASH_FUNC ## _(key, modulo);\
  KEY ## _ ## VALUE ## _pairST p_pair = find_in_ ## KEY ## _ ## VALUE ## _listST(ht[index], key); \
  return p_pair; \
} \
\
KEY ## _ ## VALUE ## _pairST find_in_ ## KEY ## _ ## VALUE ## _hash_table(KEY ## _ ## VALUE ## _hash_table h_table, KEY key) { \
  return find_in_ ## KEY ## _ ## VALUE ## _hash_table_array(h_table->ht, key, h_table->capacity); \
}\
\
void insert_ ## KEY ## _ ## VALUE ## _hash_table(KEY ## _ ## VALUE ## _hash_table *h_table, KEY key, VALUE value, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  if (!find_in_ ## KEY ## _ ## VALUE ## _hash_table_array((*h_table)->ht, key, (*h_table)->capacity)) { \
    (*h_table)->size = (*h_table)->size + 1; \
  }\
  if ((*h_table)->size >= (*h_table)->load_factor * (*h_table)->capacity) { \
    resize_and_rehash_ ## KEY ## _ ## VALUE ## _hash_table(*h_table, destruct); \
  } \
  insert_ ## KEY ## _ ## VALUE ## _hash_table_array(&((*h_table)->ht), key, value, (*h_table)->capacity); \
} \
 \
void delete_ ## KEY ## _ ## VALUE ## _hash_table_array(KEY ## _ ## VALUE ## _hash_table_array *ht, KEY key, int modulo, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  int index = HASH_FUNC ## _(key, modulo); \
  (*ht)[index] = delete_ ## KEY ## _ ## VALUE ## _listST((*ht)[index], key, destruct); \
} \
void delete_ ## KEY ## _ ## VALUE ## _hash_table(KEY ## _ ## VALUE ## _hash_table *h_table, KEY key, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  if (find_in_ ## KEY ## _ ## VALUE ## _hash_table_array((*h_table)->ht, key, (*h_table)->capacity)) { \
    delete_ ## KEY ## _ ## VALUE ## _hash_table_array(&((*h_table)->ht), key, (*h_table)->capacity, destruct); \
    (*h_table)->size = (*h_table)->size - 1; \
  } \
} \
void free_ ## KEY ## _ ## VALUE ## _hash_table(KEY ## _ ## VALUE ## _hash_table *h_table, void (*destruct) (KEY ## _ ## VALUE ## _pairST)) { \
  free_ ## KEY ## _ ## VALUE ## _hash_table_array(&((*h_table)->ht), destruct, (*h_table)->capacity); \
  free(*h_table); \
  *h_table = NULL; \
}
#endif
