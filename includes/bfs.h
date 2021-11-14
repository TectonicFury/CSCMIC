// bfs finds shortest paths
#ifndef BFS_H
#include <stdlib.h>
#include <stdio.h>
#include "vertex_indexed_graph.h"
#define BFS_H
typedef struct bfs * bfs;

#ifndef QUEUE_H
#include "queue.h"
QUEUE(int)
#endif

#ifndef STACK_H
#include "stack.h"
STACK(int)
#endif


struct bfs {
  int s;
  int *marked;
  int *edge_to;
  Graph g;
};

bfs init_bfs(Graph g, int s) {
  int_queue queue;
  init_int_queue(&queue);
  bfs p_bfs = malloc(sizeof(struct bfs));
  p_bfs->s  = s;
  p_bfs->g = g;
  p_bfs->marked = malloc(g->V * sizeof(int));
  for (size_t i = 0; i < g->V; i++) {
    p_bfs->marked[i] = 0;
  }
  p_bfs->edge_to = malloc(g->V * sizeof(int));
  p_bfs->marked[s] = 1; // mark the source

  push_int_queue(queue, s);

  while (!is_empty_int_queue(queue)) {
    int_node node = dequeue_int_queue(queue);
    int v = node->val;
    for (Ints_Bag bag = g->adj[v]; bag != NULL; bag = bag->next) {
      int w = bag->v;
      if (!p_bfs->marked[w]) {
        p_bfs->edge_to[w] = v;
        p_bfs->marked[w] = 1;
        push_int_queue(queue, w);
      }
    }
  }
  return p_bfs;
}

int is_marked_bfs(bfs p_bfs, int w) {
  return p_bfs->marked[w];
}

int_stack path_bfs(bfs p_bfs, int v) { // shortest path
  if (!p_bfs->marked[v]) return NULL;
  int_stack st;
  init_int_stack(&st);
  for (int x = v; x != p_bfs->s; x = p_bfs->edge_to[x]) {
    push_int_stack(&st, x);
  }
  return st;
}
#endif
