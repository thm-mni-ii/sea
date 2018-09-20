#include "sealib/bfs.h"
using Sealib::BFS;
using Sealib::Graph;
using Sealib::CompactArray;

void BFS::init() {
  u = 0;
  dist = 0;
  innerGray = BFS_GRAY1;
  outerGray = BFS_GRAY2;
  preprocess(0);
  color->insert(0, innerGray);
}

bool BFS::nextComponent() {
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

bool BFS::hasGrayNode() {
  // needs 4-color CD:
  // try {
  //   c.choice();
  //   return true;
  // } catch(std::exception) {
  //   return false;
  // }

  // TEMPORARY solution:
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == BFS_GRAY1 || color->get(a) == BFS_GRAY2) return true;
  }
  return false;
}
uint BFS::getGrayNode() {
  // return choice();

  // TEMPORARY solution:
  uint r;
  bool found = false;
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == innerGray) {
      r = a;
      found = true;
      break;
    }
  }
  if (!found) {
    for (uint a = 0; a < n; a++) {
      if (color->get(a) == outerGray) {
        r = a;
        found = true;
        break;
      }
    }
  }
  if (!found)
    throw std::logic_error(
        "BFS: no more gray nodes found; did you forget to call "
        "nextComponent()?");
  else
    return r;
}

bool BFS::more() { return hasGrayNode(); }

Pair BFS::next() {
  u = getGrayNode();
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
