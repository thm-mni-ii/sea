#include "sealib/iterator/edgemarker.h"

namespace Sealib {

static std::vector<bool> makeEdges(UndirectedGraph const *g) {
    std::vector<bool> bits;
    uint m = 0;
    for (uint u = 0; u < g->getOrder(); u++) {
        if (g->deg(u) == 0) {
            bits.push_back(1);
            m++;
        } else {
            for (uint k = 0; k < g->deg(u); k++) {
                bits.push_back(1);
                m++;
                bits.push_back(0);
                bits.push_back(0);
                bits.push_back(0);
                bits.push_back(0);
            }
        }
    }
    return bits;
}

static Bitset<uint8_t> makeOffset(UndirectedGraph const *g) {
    std::vector<bool> bits;
    for (uint u = 0; u < g->getOrder(); u++) {
        bits.push_back(1);
        for (uint k = 0; k < g->deg(u); k++) {
            bits.push_back(0);
        }
    }
    return Bitset<uint8_t>(bits);
}

EdgeMarker::EdgeMarker(UndirectedGraph const *graph)
    : g(graph),
      n(g->getOrder()),
      parent(g),
      edges(makeEdges(g)),
      offset(makeOffset(g)) {}

void EdgeMarker::identifyEdges() {
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            DFS::visit_nplusm(a, g, &color, &parent, DFS_NOP_PROCESS,
                                [this, &color](uint u, uint k) {
                                    if (!isInitialized(u, k)) {
                                        uint v = g->head(u, k);
                                        if (u == v) {
                                            initEdge(u, k, NONE);
                                        } else if (color.get(v) == DFS_WHITE) {
                                            initEdge(u, k, UNMARKED);
                                        } else if (color.get(v) == DFS_GRAY) {
                                            // initializing {u,v} as a back edge
                                            // with v parent of u
                                            // (closer to root)
                                            uint pk = g->mate(u, k);
                                            initEdge(g->head(u, k), pk, BACK);
                                        } else {
                                            initEdge(u, k, CROSS);
                                        }
                                    }
                                },
                                DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
        }
    }
}

void EdgeMarker::markTreeEdges() {
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            DFS::visit_nplusm(
                a, g, &color, &parent,
                [this, &a](uint u) {
                    if (u == a /*?*/ ||
                        isTreeEdge(u, static_cast<uint>(parent.get(u)))) {
                        for (uint k = 0; k < g->deg(u); k++) {
                            uint v = g->head(u, k);
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

void EdgeMarker::markParents(uint w, uint u) {
    uint k = static_cast<uint>(parent.get(w));
    uint v = w;
    // if k>=deg(w), then w is already root (?)
    while (g->head(v, k) != u && !isFullMarked(v, k)) {
        setMark(v, k, FULL);
        v = g->head(v, k);
        k = static_cast<uint>(parent.get(v));
    }
    if (g->head(v, k) == u && !isFullMarked(v, k)) setMark(v, k, HALF);
}

void EdgeMarker::initEdge(uint u, uint k, uint8_t type) {
    uint k2 = g->mate(u, k), v = g->head(u, k);
    uint ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, (type | PARENT));
    edges.insert(vi, type);
}

void EdgeMarker::setMark(uint u, uint k, uint8_t mark) {
    uint k2 = g->mate(u, k), v = g->head(u, k);
    uint ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, (edges.get(ui) & PARENT_MASK) | mark);
    edges.insert(vi, (edges.get(vi) & PARENT_MASK) | mark);
}
}  // namespace Sealib
