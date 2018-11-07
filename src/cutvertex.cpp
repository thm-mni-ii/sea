#include "sealib/cutvertex.h"
#include <cmath>
#include "sealib/choicedictionary.h"
#include "sealib/staticspacestorage.h"

using namespace Sealib;  // NOLINT

static std::vector<bool> makeEdges(BasicGraph *g) {
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

static Bitset<unsigned char> makeOffset(BasicGraph *g) {
    std::vector<bool> bits;
    for (uint u = 0; u < g->getOrder(); u++) {
        bits.push_back(1);
        for (uint k = 0; k < g->getNodeDegree(u); k++) {
            bits.push_back(0);
        }
    }
    return Bitset<unsigned char>(bits);
}

CutVertexIterator::CutVertexIterator(BasicGraph *graph)
    : g(graph),
      n(g->getOrder()),
      offset(makeOffset(g)),
      edges(makeEdges(g)),
      cc(n),
      cut(n),
      cutI(nullptr) {
          for(uint a=0; a<n; a++) {
              // we have to initialize the choice dictionary in reverse
              cc.insert(a);
              cc.remove(a);
              //cut.insert(a);
              //cut.remove(a);
          }
      }

void CutVertexIterator::setTreeEdge(uint u, uint k, bool uChild) {
    auto p = g->mate(u, k);
    uint v = std::get<0>(p), k2 = std::get<1>(p);
    uint ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, edges.get(ui) | EDGE_BIT);
    edges.insert(vi, edges.get(vi) | EDGE_BIT);
    if (!uChild) {
        edges.insert(ui, edges.get(ui) | PARENT_BIT);
    } else {
        edges.insert(vi, edges.get(vi) | PARENT_BIT);
    }
}

void CutVertexIterator::setMark(uint u, uint k, uint8_t mark) {
    auto p = g->mate(u, k);
    uint v = std::get<0>(p), k2 = std::get<1>(p);
    uint ui = edgeIndex(u) + k, vi = edgeIndex(v) + k2;
    edges.insert(ui, edges.get(ui) & (EDGE_BIT | PARENT_BIT));
    edges.insert(vi, edges.get(vi) & (EDGE_BIT | PARENT_BIT));
    edges.insert(ui, edges.get(ui) | mark);
    edges.insert(vi, edges.get(vi) | mark);
}

void CutVertexIterator::markParents(uint w, uint u,
                                    StaticSpaceStorage *parent) {
    uint k = static_cast<uint>(parent->get(w));
    uint v = g->head(w, k);
    while (v != u && !isFullMarked(w, k)) {
        setMark(w, k, FULL);
        w = v;
        k = static_cast<uint>(parent->get(w));
        v = g->head(w, k);
    }
    if (v == u) {
        if (!isFullMarked(w, k)) setMark(w, k, HALF);
    }
}

void CutVertexIterator::init_tree(CompactArray *color,
                                  StaticSpaceStorage *parent) {
    for (uint a = 0; a < n; a++) {
        if (color->get(a) == DFS_WHITE) {
            cc.insert(a);
            process_static(
                a, g, color, parent,
                [this, &parent, a](uint u) {
                    if (u != a) {
                        setTreeEdge(u, static_cast<uint>(parent->get(u)), true);
                    }
                },
                DFS_NOP_EXPLORE, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
        }
    }
}

void CutVertexIterator::init_mark(CompactArray *color,
                                  StaticSpaceStorage *parent) {
    for (uint a = 0; a < n; a++) {
        if (color->get(a) == DFS_WHITE) {
            process_static(
                a, g, color, parent,
                [this, &parent](uint u) {
                    if (isTreeEdge(u, static_cast<uint>(parent->get(u)))) {
                        for (uint k = 0; k < g->getNodeDegree(u); k++) {
                            uint v = g->head(u, k);
                            // to do: check that v is descendant of u (how?)
                            if (!isTreeEdge(u, k) && /*isParent(u, k) &&*/
                                !cc.get(v)) {
                                // {u,v} is a back edge and u is closer to root:
                                markParents(v, u, parent);
                            }
                        }
                    }
                },
                DFS_NOP_EXPLORE, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
        }
    }
}

void CutVertexIterator::init_cut() {
    for (uint u = 0; u < n; u++) {
        if (cc.get(u)) {
            // u is root of a DFS tree
            uint num = 0;
            for (uint k = 0; k < n; k++) {
                if (isTreeEdge(u, k) && isParent(u, k)) {
                    num++;
                }
                if (num > 1) {
                    cut.insert(u);
                    break;
                }
            }
        } else {
            for (uint k = 0; k < g->getNodeDegree(u); k++) {
                if (isTreeEdge(u, k) && isParent(u, k) && !isFullMarked(u, k)) {
                    cut.insert(u);
                    break;
                }
            }
        }
    }
}

void CutVertexIterator::init() {
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    std::vector<bool> bits;
    for (uint u = 0; u < n; u++) {
        bits.push_back(1);
        for (uint k = 0; k < ceil(log2(g->getNodeDegree(u) + 1)); k++) {
            bits.push_back(0);
        }
    }
    StaticSpaceStorage parent(bits);

    init_tree(&color, &parent);

    for (uint a = 0; a < n; a++) {
        color.insert(a, DFS_WHITE);
    }
    init_mark(&color, &parent);

    init_cut();
    cutI=ChoiceDictionaryIterator(&cut);
}

bool CutVertexIterator::more() { return cutI.more(); }

uint CutVertexIterator::next() { return static_cast<uint>(cutI.next()); }

bool CutVertexIterator::isCutVertex(uint u) { return cut.get(u); }
