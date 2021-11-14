// simple graph data structure implementation to focus on graph algorithms
#ifndef GRAPH_H
#define GRAPH_H
#include <stdlib.h>

typedef struct Ints_Bag *Ints_Bag;
typedef struct Graph *Graph;

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

struct Graph {
  int V; // number of vertices
  int E; // number of edges
  Ints_Bag *adj; // pointer to Ints_Bag's (i.e. array of Ints_Bag)
};

void add_edge(Graph g, int v, int w) {
  Ints_Bag bag_v = make_ints_bag(v);
  Ints_Bag bag_w = make_ints_bag(w);
  bag_w->next = g->adj[v];
  g->adj[v] = bag_w;
  bag_v->next = g->adj[w];
  g->adj[w] = bag_v;
}
Graph init_graph(int V, int E) {
  Graph g = malloc(sizeof(struct Graph));
  g->V = V;
  g->E = E;
  g->adj = malloc(V * sizeof(Ints_Bag));
  for (size_t i = 0; i < V; i++) {
    g->adj[i] = NULL;
  }
  return g;
}
#endif
