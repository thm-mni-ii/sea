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
                      uint u0) {
  std::stack<uint> *s = new std::stack<uint>;
  s->push(u0);
  while (!s->empty()) {
    uint u = s->top();
    s->pop();
    color[u] = DFS_GRAY;
    Node *un = g->getNode(u);
    if (preProcess != DFS_NOP_PROCESS) preProcess(un);
    for (uint k = 0; k < un->getDegree(); k++) {
      uint v = g->head(u, k);
      Node *vn = g->getNode(v);
      if (color[v] == DFS_WHITE) {
        if (preExplore != DFS_NOP_EXPLORE) preExplore(un, vn);
        s->push(v);
        color[v] = DFS_GRAY;
        if (postExplore != DFS_NOP_EXPLORE) postExplore(un, vn);
      }
    }
    if (postProcess != DFS_NOP_PROCESS) postProcess(un);
    color[u] = DFS_BLACK;
  }
}
void process_small(uint node, Graph *g, CompactArray *color, SegmentStack *s,
                   UserFunc1 preProcess, UserFunc2 preExplore,
                   UserFunc2 postExplore, UserFunc1 postProcess, double epsilon,
                   bool isRestoring) {
  s->push(std::make_tuple(node, 0));
  State x;
  while (!s->empty()) {
    int sr = s->pop(&x);
    if (sr == DFS_NO_MORE_NODES) {
      return;
    } else if (sr == DFS_DO_RESTORE) {
      s->saveTrailer();
      s->dropAll();
      for (uint a = 0; a < g->getOrder(); a++) {
        if (color->get(a) == DFS_GRAY) {
          color->insert(a, DFS_WHITE);
        }
      }
      process_small(node, g, color, s, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                    DFS_NOP_EXPLORE, DFS_NOP_PROCESS, epsilon, true);
    }
    uint u, k;
    u = std::get<0>(x);
    k = std::get<1>(x);
    Node *un = g->getNode(u);
    if (preProcess != DFS_NOP_PROCESS) {
      preProcess(un);
    }
    color->insert(u, DFS_GRAY);
    if (k < un->getDegree()) {
      s->push(std::make_tuple(u, k + 1));
      if (isRestoring && s->isAligned()) {
        return;
      }
      uint v = g->head(u, k);
      if (color->get(v) == DFS_WHITE) {
        Node *vn = g->getNode(v);
        if (preExplore != DFS_NOP_EXPLORE) {
          preExplore(un, vn);
        }
        s->push(std::make_tuple(v, 0));
        if (isRestoring && s->isAligned()) {
          return;
        }
        if (postExplore != DFS_NOP_EXPLORE) {
          postExplore(un, vn);
        }
      }
    } else {
      color->insert(u, DFS_BLACK);
      if (postProcess != DFS_NOP_PROCESS) {
        postProcess(un);
      }
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
void DFS::runEHKDFS(Graph *g, void (*preProcess)(Node *),
                    void (*preExplore)(Node *, Node *),
                    void (*postExplore)(Node *, Node *),
                    void (*postProcess)(Node *)) {
  unsigned int n = g->getOrder();
  double e =
      n % 2 == 0 ? 1.5 : 3;  // assume that 3/e is an integer that divides n
  unsigned q = (unsigned)ceil(
      n / log(n) * e / 6);  // 2q entries on S shall take up at most (e/3)n bits
#ifdef DFS_DEBUG
  printf("q=%u, n=%u, (e/3)n=%.0f\n", q, n, (1.5 / 3) * n);
#endif
  SegmentStack *s = new SegmentStack(n, q);
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
