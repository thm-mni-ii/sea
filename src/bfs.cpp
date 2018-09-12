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
        preexplore(u, v);
        if (color->get(v) == BFS_WHITE) {
          preprocess(v);
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
      preprocess(u);
      color.insert(u, BFS_GRAY1);
      Pair u0(u, 0);
      ConnectedComponent s =
          makeSequence(g, &color, u0, preprocess, preexplore);
      r.push_back(s);
    }
  }
  return r;
}
