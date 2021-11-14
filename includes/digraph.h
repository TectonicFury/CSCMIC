#ifndef DIGRAPH_H
#include <stdlib.h>
#define DIGRAPH_H

#ifndef GRAPH_H
typedef struct Ints_Bag *Ints_Bag;
struct Ints_Bag {
  int v; // the vertex held in the bag
  Ints_Bag next;
};

Ints_Bag make_ints_bag(int v) {
  Ints_Bag p_b = malloc(sizeof(struct Ints_Bag));
  p_b->v = v;
  p_b->next = NULL;
  return p_b;
}
#endif

typedef struct Digraph * Digraph;

struct Digraph {
  int V;
  int E;
  Ints_Bag *adj;
};

void init_digraph(Digraph *dg, int V) {
  Digraph p_dg = malloc(sizeof(struct Digraph));
  p_dg->V = V;
  p_dg->E = 0;
  p_dg->adj = malloc(V * sizeof(Ints_Bag));
  for (int i = 0; i < V; i++) {
    p_dg->adj[i] = NULL;
  }
  *dg = p_dg;
}

void add_edge_digraph(Digraph dg, int v, int w) {
  Ints_Bag bag_w = make_ints_bag(w);
  bag_w->next = dg->adj[v];
  dg->adj[v] = bag_w;
}
int num_vertex(Digraph dg) {
  return dg->V;
}
int num_edges(Digraph dg) {
  return dg->E;
}
Digraph reverse_digraph(Digraph dg) {
  Digraph p_dg;
  init_digraph(&p_dg, dg->V);
  for (int v = 0; v < dg->V; v++) {
    for (Ints_Bag bag = dg->adj[v]; bag != NULL; bag = bag->next) {
      int w = bag->v;
      add_edge_digraph(p_dg, w, v);
    }
  }
  return p_dg;
}
#endif
