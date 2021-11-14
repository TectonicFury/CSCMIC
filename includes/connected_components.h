#ifndef CONNECTED_COMP_H
#include <stdlib.h>
#include "vertex_indexed_graph.h"
#define CONNECTED_COMP_H
typedef struct CC * CC;
struct CC {
  int *marked;
  int *id;
  int count;
};

void dfs_CC(CC p_cc, Graph g, int v) ;

void init_CC(CC *p_cc, Graph g) {
  CC p = malloc(sizeof(struct CC));
  p->marked = malloc(g->V * sizeof(int));

  for (int i = 0; i < g->V; i++) {
    p->marked[i] = 0;
  }
  p->id = malloc(g->V * sizeof(int));
  p->count = 0;
  *p_cc = p;
  for (int s = 0; s < g->V; s++) {
    if (!p->marked[s]) {
      dfs_CC(p, g, s);
      p->count++;
    }
  }
}

void dfs_CC(CC p_cc, Graph g, int v) {
  p_cc->marked[v] = 1;
  p_cc->id[v] = p_cc->count;
  for (Ints_Bag bg = g->adj[v]; bg != NULL; bg = bg->next) {
    int w = bg->v;
    if (!p_cc->marked[w]) {
      dfs_CC(p_cc, g, w);
    }
  }
}

int cc_connected(CC p_cc, int v, int w) {
  return p_cc->id[v] == p_cc->id[w];
}
int component_id(CC p_cc, int v) {
  return p_cc->id[v];
}
int component_count(CC p_cc) {
  return p_cc->count;
}
#endif
