#include "sealib/dfs.h"
#include <stdio.h>
#include <stack>
#include "./segmentstack.h"

using Sealib::DFS;
using Sealib::SegmentStack;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;

static void process_standard(Graph *g, UserFunc1 preProcess,
                             UserFunc2 preExplore, UserFunc2 postExplore,
                             UserFunc1 postProcess, unsigned *color, uint u);

static void process_small(uint node, Graph *g, CompactArray *color,
                          SegmentStack *s, UserFunc1 preProcess,
                          UserFunc2 preExplore, UserFunc2 postExplore,
                          UserFunc1 postProcess);

void process_standard(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess,
                      unsigned *color, uint u0) {
  std::stack<State> *s = new std::stack<State>;
  s->push(std::make_tuple(u0, 0));
  while (!s->empty()) {
    State x = s->top();
    s->pop();
    uint u = std::get<0>(x);
    uint k = std::get<1>(x);
    if (color[u] == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color[u] = DFS_GRAY;
    }
    if (k < g->getNode(u)->getDegree()) {
      s->push(std::make_tuple(u, k + 1));
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color[v] == DFS_WHITE) {
        s->push(std::make_tuple(v, 0));
        g->getNode(v)->parent = u;
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != u0)
        postExplore(g->getNode(u)->parent, u);
      color[u] = DFS_BLACK;
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
  delete s;
}

void process_small(uint node, Graph *g, CompactArray *color, SegmentStack *s,
                   UserFunc1 preProcess, UserFunc2 preExplore,
                   UserFunc2 postExplore, UserFunc1 postProcess) {
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
                    DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
      s->pop(&x);
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
      if (s->isAligned()) return;
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) {
        s->push(std::make_tuple(v, 0));
        g->getNode(v)->parent = u;
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
      if (s->isAligned()) return;
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != node)
        postExplore(g->getNode(u)->parent, u);
      color->insert(u, DFS_BLACK);
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
}

void DFS::runStandardDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                         UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned *color = new unsigned[g->getOrder()];
  for (uint a = 0; a < g->getOrder(); a++) color[a] = DFS_WHITE;
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color[u] == DFS_WHITE) {
      process_standard(g, preProcess, preExplore, postExplore, postProcess,
                       color, u);
    }
  }
  delete[] color;
}
void DFS::runEHKDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                    UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();
  double e = 0.2;
  unsigned q = static_cast<unsigned>(ceil(
      ceil(e / 6 * n) /
      (8 *
       sizeof(State))));  // 2q entries on S shall take up at most (e/3)n bits
  unsigned qs =
      static_cast<unsigned>(ceil(n / log(n)));  // stable segment size (?)
  if (q < qs) q = qs;
  unsigned vpg = static_cast<unsigned>(ceil(3 / e));  // 3/e values per group,
                                                      // assume that 3/e is an
                                                      // integer that divides n

  // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
  SegmentStack *s = new SegmentStack(n, q, false);
  CompactArray *color = new CompactArray(n, vpg);
  for (uint a = 0; a < n; a++) color->insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == DFS_WHITE)
      process_small(a, g, color, s, preProcess, preExplore, postExplore,
                    postProcess);
  }
  delete color;
  delete s;
}
