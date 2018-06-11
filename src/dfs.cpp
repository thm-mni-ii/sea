#include "sealib/dfs.h"
#include <stdio.h>

State::State(uint pu, uint pk) {
  u = pu;
  k = pk;
}
// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void DFS::process_standard(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                           UserFunc2 postExplore, UserFunc1 postProcess,
                           uint *color, uint u) {
  color[u] = DFS_GRAY;
  Node *un = g->getNode(u);
  if (preProcess != DFS_NOP_PROCESS) preProcess(un);
  for (uint k = 0; k < un->getDegree(); k++) {
    uint v = g->head(u, k);
    Node *vn = g->getNode(v);
    if (preExplore != DFS_NOP_EXPLORE) {
      preExplore(un, vn);
    }
    if (color[v] == DFS_WHITE) {
      process_standard(g, preProcess, preExplore, postExplore, postProcess,
                       color, v);
    }
    if (postExplore != DFS_NOP_EXPLORE) {
      postExplore(un, vn);
    }
  }
  if (postProcess != DFS_NOP_PROCESS) {
    postProcess(un);
  }
  color[u] = DFS_BLACK;
}
void DFS::process_small(uint node, Graph *g, CompactArray *color,
                        UserFunc1 preProcess, UserFunc2 preExplore,
                        UserFunc2 postExplore, UserFunc1 postProcess,
                        double epsilon, bool isRestoring) {
  unsigned n = g->getOrder();
  unsigned q =
      (unsigned)ceil(n / log(n) * epsilon /
                     6);  // 2q entries on S shall take up at most (e/3)n bits
#ifdef DFS_DEBUG
  printf("q=%u, n=%u, (e/3)n=%.0f\n", q, n, (1.5 / 3) * n);
#endif
  Stack *st = new Stack;  // new Stack((unsigned)ceil(n/(double)q)),
  //*kt = new Stack;    // new Stack((unsigned)ceil(n/(double)q));
  Stack *s1 = new Stack, *s2 = new Stack;  // new Stack(q)
  // Stack *k1 = new Stack, *k2 = new Stack;
  // if (!isRestoring) {
  // st->push(0);
  // kt->push(0);
  //}
  tryPush(new State(node, 0), q, s1, s2, st);
  if (isRestoring && s2->top() == st->top()) {
#ifdef DFS_DEBUG
    printf("restoration finished: %p = %p\n", (void *)s2->top(),
           (void *)st->top());
#endif
    return;
  }
  State *x;
  uint u, k;
  while (!st->empty()) {
    try {
      x = tryPop(s1, s2, st);
    } catch (unsigned e) {
      if (e == DFS_NO_MORE_NODES) {
#ifdef DFS_DEBUG
        printf("no more nodes\n");
#endif
        return;
      } else if (e == DFS_DO_RESTORE) {
#ifdef DFS_DEBUG
        printf("(restoring ... ");
#endif
        for (uint a = 0; a < g->getOrder(); a++) {
          printf("recoloring %u?\n", a);
          if (color->get(a) == DFS_GRAY) {
            printf("YES\n");
            color->insert(a, DFS_WHITE);
          }
        }
        printf("new process(u)\n");
        for (uint a = 0; a < g->getOrder(); a++) {
          if (color->get(a) == DFS_WHITE)
            process_small(a, g, color, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                          DFS_NOP_EXPLORE, DFS_NOP_PROCESS, epsilon, true);
        }
#ifdef DFS_DEBUG
        printf("done)\n");
#endif
      }
    }
    u = x->u;
    k = x->k;
    color->insert(u, DFS_GRAY);
    Node *un = g->getNode(u);
    if (preProcess != DFS_NOP_PROCESS) {
      preProcess(un);
    }
#ifdef DFS_DEBUG
    printf("u: %u(%p), k: %u\n", u, static_cast<void *>(un), k);
    printf("deg(u): %u\n", un->getDegree());
#endif
    if (k < un->getDegree()) {
      tryPush(new State(u, k + 1), q, s1, s2, st);
#ifdef DFS_DEBUG
      printf("uAdj: %p (%u)\n", (void *)un->getAdj(), un->getAdj()->vertex);
#endif
      uint v = g->head(u, k);
#ifdef DFS_DEBUG
      printf("v: %u (%p)\n", v, (void *)g->getNode(v));
#endif
      if (color->get(v) == DFS_WHITE) {
        Node *vn = g->getNode(v);
        if (preExplore != DFS_NOP_EXPLORE) {
          preExplore(un, vn);
        }
        tryPush(new State(v, 0), q, s1, s2, st);
        if (postExplore != DFS_NOP_EXPLORE) {
          postExplore(un, vn);
        }
      }
    } else {
      color->insert(u, DFS_BLACK);
    }
    if (postProcess != DFS_NOP_PROCESS) {
      postProcess(un);
    }
  }
}
void DFS::tryPush(State *u, uint q, Stack *low, Stack *high, Stack *trailers) {
// todo: throw DFS_RESTORE_DONE if top(high)==top(trailers)
#ifdef DFS_DEBUG
  printf("-> tryPush (%u,%u) \n", u->u, u->k);
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", low->size(), high->size(),
         trailers->size());
#endif
  if (low->size() < q) {
#ifdef DFS_DEBUG
    printf(" (low segment)\n");
#endif
    low->push(u);
#ifdef DFS_DEBUG
    printf(" trailer %p => (%u,%u)\n",
           !trailers->empty() ? trailers->top() : (void *)0, u->u, u->k);
#endif
    if (trailers->size() > 0) trailers->pop();
    trailers->push(u);
  } else if (high->size() < q) {
#ifdef DFS_DEBUG
    printf(" (high segment)\n");
#endif
    high->push(u);
    trailers->pop();
    trailers->push(u);
  } else {
#ifdef DFS_DEBUG
    printf(" (drop old segment, %lu)\n", low->size());
#endif
    while (!low->empty()) {
#ifdef DFS_DEBUG
      printf(" (pop low)\n");
#endif
      low->pop();
    }
    while (!high->empty()) {
      State *a = high->top();
#ifdef DFS_DEBUG
      printf(" (mov high (%u,%u) -> low)\n", a->u, a->k);
#endif
      low->push(a);
      high->pop();
    }
  }
#ifdef DFS_DEBUG
  printf("<- tryPush \n");
#endif
}
State *DFS::tryPop(Stack *low, Stack *high, Stack *trailers) {
#ifdef DFS_DEBUG
  printf("-> tryPop \n");
#endif
  State *r = 0;
  if (!high->empty()) {
#ifdef DFS_DEBUG
    printf(" (high segment)\n");
#endif
    r = high->top();
    high->pop();
  } else if (!low->empty()) {
#ifdef DFS_DEBUG
    printf(" (low segment)\n");
#endif
    r = low->top();
    low->pop();
  } else {
    if (!trailers->empty()) {
      throw DFS_DO_RESTORE;
    } else {
      throw DFS_NO_MORE_NODES;
    }
  }
#ifdef DFS_DEBUG
  printf("<- tryPop = (%u,%u)\n", r->u, r->k);
#endif
  return r;
}

void DFS::runStandardDFS(Graph *g, void (*preProcess)(Node *),
                         void (*preExplore)(Node *, Node *),
                         void (*postExplore)(Node *, Node *),
                         void (*postProcess)(Node *)) {
  uint *color = new uint[g->getOrder()];
  for (uint u = 0; u < g->getOrder(); u++) {
    color[u] = DFS_WHITE;
  }
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color[u] == DFS_WHITE) {
      process_standard(g, preProcess, preExplore, postExplore, postProcess,
                       color, u);
    }
  }
  delete[] color;
}
void DFS::runSmallDFS(Graph *g, void (*preProcess)(Node *),
                      void (*preExplore)(Node *, Node *),
                      void (*postExplore)(Node *, Node *),
                      void (*postProcess)(Node *)) {
  unsigned n = g->getOrder();
  double e =
      n % 2 == 0 ? 1.5 : 3;  // assume that 3/e is an integer that divides n
  CompactArray *color = new CompactArray(n, e);
  for (uint a = 0; a < g->getOrder(); a++) color->insert(a, DFS_WHITE);
  printf("runSmallDFS\n");
  for (uint a = 0; a < g->getOrder(); a++) {
    printf("node %u: color %d\n", a, color->get(COMPACTARRAY_VALUE, a));
    if (color->get(a) == DFS_WHITE)
      process_small(a, g, color, preProcess, preExplore, postExplore,
                    postProcess, e, false);
  }
  delete color;
}
