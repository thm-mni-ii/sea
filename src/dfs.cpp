#include "sealib/dfs.h"
#include <stdio.h>
#include "./segmentstack.h"

static void process_standard(Graph *g, UserFunc1 preProcess,
                             UserFunc2 preExplore, UserFunc2 postExplore,
                             UserFunc1 postProcess, uint *color, uint u);

static void process_small(uint node, Graph *g, CompactArray *color,
                          SegmentStack *s, UserFunc1 preProcess,
                          UserFunc2 preExplore, UserFunc2 postExplore,
                          UserFunc1 postProcess, double epsilon,
                          bool isRestoring);

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void process_standard(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess, uint *color,
                      uint u) {
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
void process_small(uint node, Graph *g, CompactArray *color, SegmentStack *s,
                   UserFunc1 preProcess, UserFunc2 preExplore,
                   UserFunc2 postExplore, UserFunc1 postProcess, double epsilon,
                   bool isRestoring) {
  s->push(std::make_tuple(node, 0));
  if (isRestoring && s->isAligned()) {
#ifdef DFS_DEBUG
    printf("restoration finished\n");
#endif
    return;
  }
  State x;
  while (!s->empty()) {
    int e = s->pop(&x);
    if (e == DFS_NO_MORE_NODES) {
#ifdef DFS_DEBUG
      printf("no more nodes\n");
#endif
      return;
    } else if (e == DFS_DO_RESTORE) {
#ifdef DFS_DEBUG
      printf("(restoring ... ");
#endif
      s->saveTrailer();
      s->dropAll();
      for (uint a = 0; a < g->getOrder(); a++) {
        if (color->get(a) == DFS_GRAY) {
#ifdef DFS_DEBUG
          printf("recoloring %u to white\n", a);
#endif
          color->insert(a, DFS_WHITE);
        }
      }
      process_small(node, g, color, s, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                    DFS_NOP_EXPLORE, DFS_NOP_PROCESS, epsilon, true);
#ifdef DFS_DEBUG
      printf("done)\n");
#endif
    }
    uint u, k;
    u = std::get<0>(x);
    k = std::get<1>(x);
    color->insert(u, DFS_GRAY);
    Node *un = g->getNode(u);
    if (preProcess != DFS_NOP_PROCESS) {
      preProcess(un);
    }
#ifdef DFS_DEBUG
    printf("u: %u, k: %u\n", u, k);
// printf("deg(u): %u\n", un->getDegree());
#endif
    if (k < un->getDegree()) {
      s->push(std::make_tuple(u, k + 1));
      uint v = g->head(u, k);
#ifdef DFS_DEBUG
      printf("v: %u\n", v);
#endif
      if (color->get(v) == DFS_WHITE) {
        Node *vn = g->getNode(v);
        if (preExplore != DFS_NOP_EXPLORE) {
          preExplore(un, vn);
        }
        s->push(std::make_tuple(v, 0));
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
  unsigned q = (unsigned)ceil(
      n / log(n) * e / 6);  // 2q entries on S shall take up at most (e/3)n bits
#ifdef DFS_DEBUG
  printf("q=%u, n=%u, (e/3)n=%.0f\n", q, n, (1.5 / 3) * n);
#endif
  SegmentStack *s = new SegmentStack(q);
  CompactArray *color = new CompactArray(n, e);
  for (uint a = 0; a < g->getOrder(); a++) color->insert(a, DFS_WHITE);
  for (uint a = 0; a < g->getOrder(); a++) {
    if (color->get(a) == DFS_WHITE)
      process_small(a, g, color, s, preProcess, preExplore, postExplore,
                    postProcess, e, false);
  }
  delete color;
  delete s;
}
