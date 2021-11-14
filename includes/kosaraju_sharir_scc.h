#ifndef KOSARAJU_H
#define KOSARAJU_H

#include "dfs_order.h"

typedef struct Kosaraju_Sharir_SCC * Kosaraju_Sharir_SCC;

struct Kosaraju_Sharir_SCC {
  int *marked;
  int *id;
  int count;
};

void dfs_KS_SCC(Kosaraju_Sharir_SCC ks_scc, Digraph dg, int v);

void init_KS_SCC(Kosaraju_Sharir_SCC *ks_scc, Digraph dg) {
  Kosaraju_Sharir_SCC k = malloc(sizeof(struct Kosaraju_Sharir_SCC));
  k->marked = malloc(dg->V * sizeof(int));
  k->id = malloc(dg->V * sizeof(int));
  k->count = 0;

  for (int v = 0; v < dg->V; v++) {
    k->marked[v] = 0;
  }
  Dfs_Order order;
  init_dfs_order(&order, reverse_digraph(dg));
  int_stack rev_post = order->reverse_post;
  for (int_stack st = pop_int_stack(&rev_post); st != NULL; st = pop_int_stack(&rev_post)) {
    int s = st->val;
    if (!k->marked[s]) {
      dfs_KS_SCC(k, dg, s);
      k->count++;
    }
  }
  *ks_scc = k;
}
void dfs_KS_SCC(Kosaraju_Sharir_SCC ks_scc, Digraph dg, int v) {
  ks_scc->marked[v] = 1;
  ks_scc->id[v] = ks_scc->count;
  for (Ints_Bag bag = dg->adj[v]; bag != NULL; bag = bag->next) {
    int w = bag->v;
    if (!ks_scc->marked[w]) {
      dfs_KS_SCC(ks_scc, dg, w);
    }
  }
}
int is_strongly_connected(Kosaraju_Sharir_SCC k, int v, int w) {
  return k->id[v] == k->id[w];
}
int id_KS_SCC(Kosaraju_Sharir_SCC k, int v) {
  return k->id[v];
}
int count_KS_SCC(Kosaraju_Sharir_SCC k) {
  return k->count;
}
#endif
