// dfs doesn't help with shortest paths. it finds any path.
// for cycle detection, use dfs visit pattern storing the parent and if a neighbour is marked and is not parent, there is a cycle
// symbol graph contains a hash table containing string keys paired with integer indexes, another array is maintained to get string keys for integer index, 2 pass 
#ifndef DFS_H
#include <stdlib.h>
#include <stdio.h>
#include "vertex_indexed_graph.h"
#define DFS_H
typedef struct dfs * dfs;

#ifndef STACK_H
#include "stack.h"
STACK(int)
#endif

struct dfs {
  int count;
  int s;
  int *marked;
  int *edge_to;
  Graph g;
};

dfs init_dfs(Graph g, int s) {
  dfs p_dfs = malloc(sizeof(struct dfs));
  p_dfs->s  = s;
  p_dfs->g = g;
  p_dfs->marked = malloc(g->V * sizeof(int));
  p_dfs->edge_to = malloc(g->V * sizeof(int));
  for (size_t i = 0; i < g->V; i++) {
    p_dfs->marked[i] = 0;
  }
  return p_dfs;
}

void df_search(dfs p_dfs, Graph g, int v) {
  p_dfs->marked[v] = 1;
  p_dfs->count++;
  Ints_Bag bag = g->adj[v];
  while (bag) {
    int w = bag->v;
    // printf("marked val = %d\n", p_dfs->marked[w]);
    if (!p_dfs->marked[w]) {
      p_dfs->edge_to[w] = v;
      df_search(p_dfs, g, w);
      bag = bag->next;
      continue;
    }
    bag = bag->next;
  }
}

int is_marked(dfs p_dfs, int w) {
  return p_dfs->marked[w];
}

int count(dfs p_dfs) {
  return p_dfs->count;
}

int_stack path_dfs(dfs p_dfs, int v) {
  if (!p_dfs->marked[v]) return NULL;
  int_stack st;
  init_int_stack(&st);
  for (int x = v; x != p_dfs->s; x = p_dfs->edge_to[x]) {
    push_int_stack(&st, x);
  }
  return st;
}
#endif
