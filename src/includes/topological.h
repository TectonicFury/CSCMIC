#ifndef TOPOLOGICAL_H
#define TOPOLOGICAL_H
#include <stdlib.h>
#include "directed_cycle.h"

typedef struct Toplogical_Sort * Toplogical_Sort;
struct Toplogical_Sort {
  int_stack reverse_post_order;
};

void init_toplogical_sort(Toplogical_Sort *ts, Digraph dg) {
  Toplogical_Sort p_ts = malloc(sizeof(struct Toplogical_Sort));
  init_int_stack(&p_ts->reverse_post_order);
  Directed_Cycle dc;
  init_directed_cycle(&dc, dg);
  if (!dc->cycle) {
    Dfs_Order ord;
    init_dfs_order(&ord, dg);
    p_ts->reverse_post_order = ord->reverse_post;
  }
  *ts = p_ts;
}
#endif
