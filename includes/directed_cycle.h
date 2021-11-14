#ifndef DIRECTED_CYCLE_H
#define DIRECTED_CYCLE_H
#include <stdlib.h>
#ifndef STACK_H
#include "stack.h"
STACK(int)
#endif

#include "digraph.h"

typedef struct Directed_Cycle * Directed_Cycle;

struct Directed_Cycle {
  int *marked;
  int *edge_to;
  int_stack cycle;
  int *on_stack;
};

void directed_cycle_dfs(Directed_Cycle dc, Digraph dg);

void init_directed_cycle(Directed_Cycle *dc, Digraph dg) {
  Directed_Cycle temp_dc = malloc(sizeof(struct Directed_Cycle));
  temp_dc->marked = malloc(dg->V * sizeof(int));
  temp_dc->edge_to = malloc(dg->V * sizeof(int));
  temp_dc->on_stack = malloc(dg->V * sizeof(int));
  // initialisation
  for (int i = 0; i < dg->V; i++) {
    temp_dc->marked[i] = 0;
    temp_dc->edge_to[i] = 0;
    temp_dc->on_stack[i] = 0;
  }
  int_stack st;
  init_int_stack(&st);
  temp_dc->cycle = st;
  *dc = temp_dc;
  directed_cycle_dfs(*dc, dg);
}
void aux_directed_cycle_dfs(Directed_Cycle dc, Digraph dg, int v){
  dc->on_stack[v] = 1;
  dc->marked[v] = 1;
  for (Ints_Bag bag = dg->adj[v]; bag != NULL; bag = bag->next) {
    int w = bag->v;
    if (dc->cycle) return;
    else if (!dc->marked[w]) {
      dc->edge_to[w] = v;
      aux_directed_cycle_dfs(dc, dg, w);
    } else if (dc->on_stack[w]) {
      for (int x = v; x != w; x =  dc->edge_to[x]) {
        push_int_stack(&(dc->cycle), x);
      }
      push_int_stack(&(dc->cycle), w);
      push_int_stack(&(dc->cycle), v);
    }
  }
  dc->on_stack[v] = 0;
}
void directed_cycle_dfs(Directed_Cycle dc, Digraph dg) {
  for (int v = 0; v < dg->V; v++) {
    if (!dc->marked[v]) {
      aux_directed_cycle_dfs(dc, dg, v);
    }
  }
}
#endif
