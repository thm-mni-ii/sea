#include "sealib/bfs.h"
using Sealib::BFS;
using Sealib::ConnectedComponent;
using Sealib::Graph;
using Sealib::CompactArray;

static ConnectedComponent makeSequence(Graph *g, CompactArray *color, Pair u0,
                                       UserFunc1 preprocess,
                                       UserFunc2 preexplore) {
  uint dist = 0;
  uint innerGray = BFS_GRAY1, outerGray = BFS_GRAY2;
  ConnectedComponent s;
  s.push(u0);
  for (uint c = 0; c < s.length();) {
    Pair h = s.get(c);
    uint u = h.head();
    if (color->get(u) == innerGray) {
      for (uint k = 0; k < g->getNodeDegree(u); k++) {
        uint v = g->head(u, k);
        if (preexplore != BFS_NOP_EXPLORE) preexplore(u, v);
        if (color->get(v) == BFS_WHITE) {
          if (preprocess != BFS_NOP_PROCESS) preprocess(v);
          color->insert(v, outerGray);
          s.push(Pair(v, dist + 1));
        }
      }
      color->insert(u, BFS_BLACK);
      c++;
    } else if (color->get(u) == outerGray) {
      innerGray = innerGray == BFS_GRAY1 ? BFS_GRAY2 : BFS_GRAY1;
      outerGray = outerGray == BFS_GRAY2 ? BFS_GRAY1 : BFS_GRAY2;
      dist++;
    } else {
      throw std::logic_error("BFS: non-gray color in sequence");
    }
  }
  return s;
}

std::vector<ConnectedComponent> BFS::run(Graph *g, UserFunc1 preprocess,
                                         UserFunc2 preexplore) {
  uint n = g->getOrder();
  std::vector<ConnectedComponent> r;
  CompactArray color(n, 10);
  for (uint a = 0; a < n; a++) color.insert(a, BFS_WHITE);
  for (uint u = 0; u < n; u++) {
    if (color.get(u) == BFS_WHITE) {
      if (preprocess != BFS_NOP_PROCESS) preprocess(u);
      color.insert(u, BFS_GRAY1);
      Pair u0(u, 0);
      ConnectedComponent s =
          makeSequence(g, &color, u0, preprocess, preexplore);
      r.push_back(s);
    }
  }
  return r;
}

void BFS::init() {
  u = 0;
  dist = 0;
  innerGray = BFS_GRAY1;
  outerGray = BFS_GRAY2;
  preprocess(0);
  color->insert(0, innerGray);
}

bool BFS::reinit() {
  bool found = false;
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == BFS_WHITE) {
      u = a;
      found = true;
      dist = 0;
      innerGray = BFS_GRAY1;
      outerGray = BFS_GRAY2;
      preprocess(u);
      color->insert(u, innerGray);
      break;
    }
  }
  return found;
}

bool BFS::more() {
  // REPLACE with c-color CD:
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == BFS_GRAY1 || color->get(a) == BFS_GRAY2) return true;
  }
  return false;
}

Pair BFS::next() {
  // uint u = getNextGray();
  //          ^ choice()

  // TEMPORARY solution:
  bool found = false;
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == innerGray) {
      u = a;
      found = true;
      break;
    }
  }
  if (!found) {
    for (uint a = 0; a < n; a++) {
      if (color->get(a) == outerGray) {
        u = a;
        found = true;
        break;
      }
    }
  }
  if (!found) throw std::logic_error("BFS.next(): no gray node found");

  if (color->get(u) == innerGray) {
    for (uint k = 0; k < g->getNodeDegree(u); k++) {
      uint v = g->head(u, k);
      if (preexplore != BFS_NOP_EXPLORE) preexplore(u, v);
      if (color->get(v) == BFS_WHITE) {
        if (preprocess != BFS_NOP_PROCESS) preprocess(v);
        color->insert(v, outerGray);
      }
    }
    color->insert(u, BFS_BLACK);
  } else if (color->get(u) == outerGray) {
    innerGray = innerGray == BFS_GRAY1 ? BFS_GRAY2 : BFS_GRAY1;
    outerGray = outerGray == BFS_GRAY2 ? BFS_GRAY1 : BFS_GRAY2;
    dist++;
  }
  return Pair(u, dist);
}

BFS::BFS(Graph *graph, UserFunc1 pp, UserFunc2 pe)
    : g(graph),
      n(g->getOrder()),
      color(new CompactArray(n, 10)),
      preprocess(pp),
      preexplore(pe) {
  for (uint a = 0; a < n; a++) color->insert(a, BFS_WHITE);
}

BFS::~BFS() { delete color; }
