#include "sealib/iterator/edgemarker.h"

namespace Sealib {

static std::vector<bool> makeOffset(UndirectedGraph const &g) {
    std::vector<bool> bits;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        bits.push_back(1);
        for (uint64_t k = 0; k < g.deg(u); k++) {
            bits.push_back(0);
        }
    }
    return bits;
}

EdgeMarker::EdgeMarker(UndirectedGraph const &graph)
    : g(graph),
      n(g.getOrder()),
      parent(g),
      edges(g, 4, true),
      offset(Bitset<uint8_t>(makeOffset(g))) {}

void EdgeMarker::identifyEdges() {
    CompactArray color(n, 3);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            DFS::visit_nplusm(a, g, &color, &parent, DFS_NOP_PROCESS,
                              [this, &color](uint64_t u, uint64_t k) {
                                  if (!isInitialized(u, k)) {
                                      uint64_t v = g.head(u, k);
                                      if (u == v) {
                                          initEdge(u, k, NONE);
                                      } else if (color.get(v) == DFS_WHITE) {
                                          initEdge(u, k, UNMARKED);
                                      } else if (color.get(v) == DFS_GRAY) {
                                          // initializing {u,v} as a back edge
                                          // with v parent of u
                                          // (closer to root)
                                          uint64_t pk = g.mate(u, k);
                                          initEdge(g.head(u, k), pk, BACK);
                                      } else {
                                          // cannot happen (no cross/fwd edges
                                          // in undirected graphs)
                                      }
                                  }
                              },
                              DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
        }
    }
}

void EdgeMarker::markTreeEdges() {
    CompactArray color(n, 3);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            DFS::visit_nplusm(
                a, g, &color, &parent,
                [this, &a](uint64_t u) {
                    if (u == a /*?*/ || isTreeEdge(u, parent.get(u))) {
                        for (uint64_t k = 0; k < g.deg(u); k++) {
                            uint64_t v = g.head(u, k);
                            if (isBackEdge(u, k) && isParent(u, k)) {
                                // {u,v} is a back edge and u is closer to root:
                                markParents(v, u);
                            }
                        }
                    }
                },
                DFS_NOP_EXPLORE, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
        }
    }
}

void EdgeMarker::markParents(uint64_t w, uint64_t u) {
    uint64_t k = parent.get(w);
    uint64_t v = w;
    // if k>=deg(w), then w is already root (?)
    while (g.head(v, k) != u && !isFullMarked(v, k)) {
        setMark(v, k, FULL);
        v = g.head(v, k);
        k = parent.get(v);
    }
    if (g.head(v, k) == u && !isFullMarked(v, k)) setMark(v, k, HALF);
}

void EdgeMarker::initEdge(uint64_t u, uint64_t k, uint8_t type) {
    uint64_t k2 = g.mate(u, k), v = g.head(u, k);
    uint64_t ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, (type | PARENT));
    edges.insert(vi, type);
}

void EdgeMarker::setMark(uint64_t u, uint64_t k, uint8_t mark) {
    uint64_t k2 = g.mate(u, k), v = g.head(u, k);
    uint64_t ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, (edges.get(ui) & PARENT_MASK) | mark);
    edges.insert(vi, (edges.get(vi) & PARENT_MASK) | mark);
}
}  // namespace Sealib
