#include "sealib/cutvertex.h"
#include <cmath>
#include "sealib/choicedictionary.h"
#include "sealib/staticspacestorage.h"

using namespace Sealib;  // NOLINT

static std::vector<bool> makeEdges(BasicGraph *g) {
    std::vector<bool> bits;
    for (uint u = 0; u < g->getOrder(); u++) {
        bits.push_back(1);
        for (uint k = 0; k < g->getNodeDegree(u); k++) {
            bits.push_back(0);
            bits.push_back(0);
            bits.push_back(0);
            bits.push_back(0);
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
      cutI(nullptr) {}

void CutVertexIterator::setTreeEdge(uint u, uint v) {
    uint ui = nodeIndex(u) + findEdge(u, v), vi = nodeIndex(v) + findEdge(v, u);
    edges.insert(ui, edges.get(ui) | EDGE_BIT);
    edges.insert(vi, edges.get(vi) | EDGE_BIT);
    edges.insert(vi, edges.get(vi) | DIRECTION_BIT);
}

void CutVertexIterator::setMark(uint u, uint v, uint8_t mark) {
    uint ui = nodeIndex(u) + findEdge(u, v), vi = nodeIndex(v) + findEdge(v, u);
    edges.insert(ui, edges.get(ui) & (EDGE_BIT | DIRECTION_BIT));
    edges.insert(vi, edges.get(vi) & (EDGE_BIT | DIRECTION_BIT));
    edges.insert(ui, edges.get(ui) | mark);
    edges.insert(vi, edges.get(vi) | mark);
}

void CutVertexIterator::markParents(uint w, uint u,
                                    StaticSpaceStorage *parent) {
    uint k = static_cast<uint>(parent->get(w));
    uint v = g->head(w, k);
    while (v != u && !isFullMarked(w, v)) {
        setMark(w, v, FULL);
        w = v;
        k = static_cast<uint>(parent->get(w));
        v = g->head(w, k);
    }
    if (v == u) {
        setMark(w, u, HALF);
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
    for (uint a = 0; a < n; a++) {
        cc.insert(a);
        process_static(a, g, &color, &parent, DFS_NOP_PROCESS,
                       [this, &color](uint u, uint v) {
                           if (color.get(v) == DFS_WHITE) {
                               setTreeEdge(u, v);
                           }
                       },
                       DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
    }

    for (uint a = 0; a < n; a++) {
        color.insert(a, DFS_WHITE);
    }
    for (uint a = 0; a < n; a++) {
        bool doScan = false;
        process_static(
            a, g, &color, &parent,
            [this, &doScan, &parent](uint u) {
                if (doScan) {
                    doScan = false;
                    for (uint k = 0; k < g->getNodeDegree(u); k++) {
                        uint v = g->head(u, k);
                        uint64_t e = getEdgeData(u, v);
                        if ((e & EDGE_BIT) == 0 && (e & DIRECTION_BIT) == 0) {
                            // {u,v} is a back edge and u is closer to root:
                            markParents(v, u, &parent);
                        }
                    }
                }
            },
            [this, &color, &doScan](uint u, uint v) {
                if (color.get(v) == DFS_WHITE && isTreeEdge(u, v)) {
                    doScan = true;
                }
            },
            DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
    }

    uint u = 0;
    while (u < n) {
        if (cc.get(u)) {
            uint num = 0;
            for (uint k = 0; k < n; k++) {
                if (isTreeEdge(u, g->head(u, k))) {
                    num++;
                }
                if (num > 1) {
                    cut.insert(u);
                    break;
                }
            }
        } else {
            for (uint k = 0; k < g->getNodeDegree(u); k++) {
                if (!isFullMarked(u, g->head(u, k))) {
                    cut.insert(u);
                    break;
                }
            }
        }
        u++;
    }
    cutI = ChoiceDictionaryIterator(&cut);
}

bool CutVertexIterator::more() { return cutI.more(); }

uint CutVertexIterator::next() { return static_cast<uint>(cutI.next()); }

bool CutVertexIterator::isCutVertex(uint u) { return cut.get(u); }
