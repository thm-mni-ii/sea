#include "sealib/dfs.h"
#include <stdio.h>
#include <stack>
#include "./segmentstack.h"

using Sealib::DFS;
using Sealib::SegmentStack;
using Sealib::CompactArray;

static void process_standard(Graph *g, UserFunc1 preProcess,
                             UserFunc2 preExplore, UserFunc2 postExplore,
                             UserFunc1 postProcess, unsigned *color, uint u);

static void process_small(uint node, Graph *g, CompactArray *color,
                          SegmentStack *s, UserFunc1 preProcess,
                          UserFunc2 preExplore, UserFunc2 postExplore,
                          UserFunc1 postProcess, double epsilon);

using Sealib::DFS;
using Sealib::Node;

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void process_standard(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess,
                      unsigned *color, uint u0) {
  std::stack<uint> *s = new std::stack<uint>;
  s->push(u0);
  while (!s->empty()) {
    uint u = s->top();
    s->pop();
    if (preProcess != DFS_NOP_PROCESS) preProcess(u);
    color[u] = DFS_GRAY;
    for (uint k = 0; k < g->getNode(u)->getDegree(); k++) {
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color[v] == DFS_WHITE) {
        s->push(v);
        color[v] = DFS_GRAY;
      }
      if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
    }
    if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    color[u] = DFS_BLACK;
  }
}
void process_small(uint node, Graph *g, CompactArray *color, SegmentStack *s,
                   UserFunc1 preProcess, UserFunc2 preExplore,
                   UserFunc2 postExplore, UserFunc1 postProcess,
                   double epsilon) {
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
                    DFS_NOP_EXPLORE, DFS_NOP_PROCESS, epsilon);
      sr = s->pop(&x);
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
      if (s->isAligned()) {
        return;
      }
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) s->push(std::make_tuple(v, 0));
      if (s->isAligned()) {
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
  unsigned *color = new unsigned[g->getOrder()];
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color[u] == DFS_WHITE) {
      process_standard(g, preProcess, preExplore, postExplore, postProcess,
                       color, u);
    }
  }
}
void DFS::runEHKDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                    UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();
  // assume that 3/e is an integer that divides n:
  // double e = n % 2 == 0 ? 1.5 : 3;
  double e = 0.2;
  // 2q entries on S shall take up at most (e/3)n bits:
  unsigned q = static_cast<unsigned>(ceil(e / 6 * n) / sizeof(State));

  // unsigned q=n;   /* uncomment to disable restoration */
  printf("q=%u, n=%u, (e/3)n=%.0f\n", q, n, (e / 3) * n);
  SegmentStack *s = new SegmentStack(n, q, false);
  CompactArray *color = new CompactArray(n, e);
  for (uint a = 0; a < n; a++) color->insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == DFS_WHITE)
      process_small(a, g, color, s, preProcess, preExplore, postExplore,
                    postProcess, e);
  }
  delete color;
  delete s;
}
