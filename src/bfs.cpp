#include "sealib/bfs.h"
using Sealib::BFS;
using Sealib::ConnectedComponent;
using Sealib::Graph;
using Sealib::CompactArray;

static void makeSequence(Graph *g, CompactArray *color, ConnectedComponent *s,
                         UserFunc1 preprocess, UserFunc2 preexplore) {
  uint c = 0, dist = 0;
  uint innerGray = BFS_GRAY1, outerGray = BFS_GRAY2;
  while (c < s->length()) {
    Pair h = s->get(c);
    uint u = std::get<0>(h);
    if (color->get(u) == innerGray) {
      for (uint k = 0; k < g->getNode(u)->getDegree(); k++) {
        uint v = g->head(u, k);
        if (color->get(v) == BFS_WHITE) {
          preexplore(u, v);
          preprocess(v);
          color->insert(v, outerGray);
          s->push(std::make_tuple(v, dist + 1));
        }
      }
      Pair th = s->get(c + 1);
      uint tu = std::get<0>(th);
      if (color->get(tu) == outerGray) {
        innerGray = innerGray == BFS_GRAY1 ? BFS_GRAY2 : BFS_GRAY1;
        outerGray = outerGray == BFS_GRAY2 ? BFS_GRAY1 : BFS_GRAY2;
        dist++;
      }
      c++;
    }
  }
}

std::vector<ConnectedComponent> BFS::run(Graph *g, UserFunc1 preprocess,
                                         UserFunc2 preexplore) {
  uint n = g->getOrder();
  std::vector<ConnectedComponent> r(1);
  CompactArray color(n, 4);
  for (uint a = 0; a < n; a++) color.insert(a, BFS_WHITE);
  for (uint u = 0; u < n; u++) {
    if (color.get(u) == BFS_WHITE) {
      ConnectedComponent s;
      preprocess(u);
      color.insert(u, BFS_GRAY1);
      s.push(std::make_tuple(u, 0));
      makeSequence(g, &color, &s, preprocess, preexplore);
      r.push_back(s);
    }
  }
  return r;
}
