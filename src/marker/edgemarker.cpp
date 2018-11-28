#include "sealib/iterator/edgemarker.h"

namespace Sealib {

static std::vector<bool> makeEdges(UndirectedGraph *g) {
    std::vector<bool> bits;
    uint m = 0;
    for (uint u = 0; u < g->getOrder(); u++) {
        if (g->getNodeDegree(u) == 0) {
            bits.push_back(1);
            m++;
        } else {
            for (uint k = 0; k < g->getNodeDegree(u); k++) {
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

static Bitset<uint8_t> makeOffset(UndirectedGraph *g) {
    std::vector<bool> bits;
    for (uint u = 0; u < g->getOrder(); u++) {
        bits.push_back(1);
        for (uint k = 0; k < g->getNodeDegree(u); k++) {
            bits.push_back(0);
        }
    }
    return Bitset<uint8_t>(bits);
}

EdgeMarker::EdgeMarker(UndirectedGraph *graph)
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
            DFS::process_static(
                a, g, &color, &parent, DFS_NOP_PROCESS,
                [this, &color](uint u, uint k) {
                    if (!isInitialized(u, k)) {
                        uint v = g->head(u, k);
                        if (color.get(v) == DFS_WHITE) {
                            initEdge(u, k, UNMARKED);
                        } else if (color.get(v) == DFS_GRAY) {
                            // initializing {u,v} as a back edge with v parent
                            // of u
                            // (closer to root)
                            std::tuple<uint, uint> p = g->mate(u, k);
                            initEdge(std::get<0>(p), std::get<1>(p), BACK);
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
            DFS::process_static(
                a, g, &color, &parent,
                [this, a](uint u) {
                    if (u == a /*?*/ ||
                        isTreeEdge(u, static_cast<uint>(parent.get(u)))) {
                        for (uint k = 0; k < g->getNodeDegree(u); k++) {
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
    if (k < g->getNodeDegree(w)) {
        // if k>deg(w), then w is already root (?)
        uint v = g->head(w, k);  // assert(isTreeEdge(w,k));
        while (v != u && !isFullMarked(w, k)) {
            setMark(w, k, FULL);
            w = v;
            k = static_cast<uint>(parent.get(w));
            v = g->head(w, k);
        }
        if (v == u) {
            if (!isFullMarked(w, k)) setMark(w, k, HALF);
        }
    }
}

void EdgeMarker::initEdge(uint u, uint k, uint8_t type) {
    auto p = g->mate(u, k);
    uint v = std::get<0>(p), k2 = std::get<1>(p);
    uint ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, edges.get(ui) | type);
    edges.insert(vi, edges.get(vi) | type);
    edges.insert(ui, edges.get(ui) | PARENT);
}

void EdgeMarker::setMark(uint u, uint k, uint8_t mark) {
    auto p = g->mate(u, k);
    uint v = std::get<0>(p), k2 = std::get<1>(p);
    uint ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, edges.get(ui) & PARENT_MASK);
    edges.insert(vi, edges.get(vi) & PARENT_MASK);
    edges.insert(ui, edges.get(ui) | mark);
    edges.insert(vi, edges.get(vi) | mark);
}
}  // namespace Sealib
