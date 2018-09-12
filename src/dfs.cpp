#include "sealib/dfs.h"
#include "./segmentstack.h"
#include "./inplacerunner.h"

using Sealib::DFS;
using Sealib::SegmentStack;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;

static void process_standard(uint u0, Graph *g, uint *color,
                             UserFunc1 preProcess, UserFunc2 preExplore,
                             UserFunc2 postExplore, UserFunc1 postProcess) {
  std::stack<Pair> *s = new std::stack<Pair>;
  s->push(Pair(u0, 0));
  while (!s->empty()) {
    Pair x = s->top();
    s->pop();
    uint u = x.head();
    uint k = x.tail();
    if (color[u] == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color[u] = DFS_GRAY;
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color[v] == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != u0) {
        uint pu = s->top().head();
        postExplore(pu, u);
      }
      color[u] = DFS_BLACK;
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
  delete s;
}

static void restore_small(uint, Graph *, CompactArray *, SegmentStack *);

static void process_small(uint u0, Graph *g, CompactArray *color,
                          SegmentStack *s, UserFunc1 preProcess,
                          UserFunc2 preExplore, UserFunc2 postExplore,
                          UserFunc1 postProcess) {
  s->push(Pair(u0, 0));
  Pair x;
  while (!s->empty()) {
    int sr = s->pop(&x);
    if (sr == DFS_NO_MORE_NODES) {
      return;
    }
    uint u, k;
    u = x.head();
    k = x.tail();
    if (color->get(u) == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color->insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      if (s->isAligned()) return;
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
      if (s->isAligned()) return;
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != u0) {
        Pair px;
        sr = s->pop(&px);
        if (sr == DFS_DO_RESTORE) {
          restore_small(u0, g, color, s);
          s->pop(&px);
        }
        uint pu = px.head();
        postExplore(pu, u);
        s->push(px);
      }
      color->insert(u, DFS_BLACK);
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
}

static void restore_small(uint u0, Graph *g, CompactArray *color,
                          SegmentStack *s) {
  s->saveTrailer();
  s->dropAll();
  for (uint a = 0; a < g->getOrder(); a++) {
    if (color->get(a) == DFS_GRAY) {
      color->insert(a, DFS_WHITE);
    }
  }
  process_small(u0, g, color, s, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
}

void DFS::standardDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess) {
  uint *color = new uint[g->getOrder()];
  for (uint a = 0; a < g->getOrder(); a++) color[a] = DFS_WHITE;
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color[u] == DFS_WHITE) {
      process_standard(u, g, color, preProcess, preExplore, postExplore,
                       postProcess);
    }
  }
  delete[] color;
}

void DFS::nBitDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                  UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();
  double e = 0.2;
  unsigned q = static_cast<unsigned>(ceil(
      ceil(e / 6 * n) /
      (8 *
       sizeof(Pair))));  // 2q entries on S shall take up at most (e/3)n bits
  unsigned qs = 3;       // stable segment size (?)
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


void DFS::runLinearTimeInplaceDFS(uint *graph, UserFunc1 preProcess,
                                  UserFunc1 postProcess, uint startVertex) {
  auto *ilDFSRunner =
      new LinearTimeInplaceDFSRunner(graph, preProcess, postProcess);
  ilDFSRunner->run(startVertex);
  delete ilDFSRunner;
}
