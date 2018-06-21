#include "sealib/dfs.h"
#include <stdio.h>
#include <stack>
#include "./segmentstack.h"

static void process_standard(Graph *g, UserFunc1 preProcess,
                             UserFunc2 preExplore, UserFunc2 postExplore,
                             UserFunc1 postProcess, CompactArray *color,
                             uint u);

static void process_small(uint node, Graph *g, CompactArray *color,
                          SegmentStack *s, UserFunc1 preProcess,
                          UserFunc2 preExplore, UserFunc2 postExplore,
                          UserFunc1 postProcess, double epsilon,
                          bool isRestoring);

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void process_standard(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess,
                      CompactArray *color, uint u0) {
  std::stack<uint> *s = new std::stack<uint>;
  s->push(u0);
  while (!s->empty()) {
    uint u = s->top();
    s->pop();
    if (preProcess != DFS_NOP_PROCESS) preProcess(u);
    color->insert(u, DFS_GRAY);
    for (uint k = 0; k < g->getNode(u)->getDegree(); k++) {
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) s->push(v);
      color->insert(v, DFS_GRAY);
      if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
    }
    if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    color->insert(u, DFS_BLACK);
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
    if (color->get(u) == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color->insert(u, DFS_GRAY);
    }
    if (k < g->getNode(u)->getDegree()) {
      s->push(std::make_tuple(u, k + 1));
      if (isRestoring && s->isAligned()) {
        return;
      }
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) s->push(std::make_tuple(v, 0));
      if (isRestoring && s->isAligned()) {
        return;
      }
      if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
    } else {
      color->insert(u, DFS_BLACK);
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
}

void DFS::runStandardDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                         UserFunc2 postExplore, UserFunc1 postProcess) {
  CompactArray *color = new CompactArray(g->getOrder(), 1.5);
  for (uint u = 0; u < g->getOrder(); u++) {
    color->insert(u, DFS_WHITE);
  }
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color->get(u) == DFS_WHITE) {
      process_standard(g, preProcess, preExplore, postExplore, postProcess,
                       color, u);
    }
  }
  delete color;
}
void DFS::runEHKDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                    UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();
  double e = n % 2 == 0 ? 1.5 : 3;  // assume that 3/e is an integer that
                                    // divides n 0.001;
  unsigned q = (unsigned)ceil(
      n / log(n) * e / 6);  // 2q entries on S shall take up at most (e/3)n bits
  // unsigned q=n;   /* uncomment to disable restoration */
  printf("q=%u, n=%u, (e/3)n=%.0f\n", q, n, (1.5 / 3) * n);
  SegmentStack *s = new SegmentStack(n, q, false);
  CompactArray *color = new CompactArray(n, e);
  for (uint a = 0; a < g->getOrder(); a++) color->insert(a, DFS_WHITE);
  for (uint a = 0; a < g->getOrder(); a++) {
    if (color->get(a) == DFS_WHITE)
      process_small(a, g, color, s, preProcess, preExplore, postExplore,
                    postProcess, e, true);
  }
  delete color;
  delete s;
}
