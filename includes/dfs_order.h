// pre, post and reverse post order for a digraph
#ifndef DFS_ORDER_H
#define DFS_ORDER_H
typedef struct Dfs_Order * Dfs_Order;

#ifndef QUEUE_H
#include "queue.h"
QUEUE(int)
#endif

#ifndef STACK_H
#include "stack.h"
STACK(int)
#endif

#include "digraph.h"

struct Dfs_Order {
  int *marked;
  int_queue pre;
  int_queue post;
  int_stack reverse_post;
};

void dfs_order(Dfs_Order ord, Digraph dg, int v) {
  push_int_queue(ord->pre, v);
  ord->marked[v] = 1;
  for (Ints_Bag bag = dg->adj[v]; bag != NULL; bag = bag->next) {
    int w = bag->v;
    if (!ord->marked[w]) {
      dfs_order(ord, dg, w);
    }
  }
  push_int_queue(ord->post, v);
  push_int_stack(&ord->reverse_post, v);
}

void init_dfs_order(Dfs_Order *ord, Digraph dg) {
  Dfs_Order p_ord = malloc(sizeof(struct Dfs_Order));
  init_int_queue(&p_ord->pre);
  init_int_queue(&p_ord->post);
  init_int_stack(&p_ord->reverse_post);
  p_ord->marked = malloc(dg->V * sizeof(int));
  for (int v = 0; v < dg->V; v++) {
    p_ord->marked[v] = 0;
  }
  for (int v = 0; v < dg->V; v++) {
    if (!p_ord->marked[v]) {
      dfs_order(p_ord, dg, v);
    }
  }
  *ord = p_ord;
}

int_queue pre_dfs_order(Dfs_Order ord) {
  return ord->pre;
}

int_queue post_dfs_order(Dfs_Order ord) {
  return ord->post;
}

int_stack reverse_post_dfs_order(Dfs_Order ord) {
  return ord->reverse_post;
}

#endif
