#include "sealib/cutvertex.h"
#include <cmath>
#include "sealib/choicedictionary.h"
#include "sealib/staticspacestorage.h"

using namespace Sealib;  // NOLINT

static std::vector<bool> makeBits(BasicGraph *g, std::vector<uint> *offs) {
    std::vector<bool> bits;
    uint offset = 0;
    for (uint u = 0; u < g->getOrder(); u++) {
        offs->push_back(offset);
        bits.push_back(1);
        for (uint k = 0; k < g->getNodeDegree(u); k++) {
            bits.push_back(0);
            bits.push_back(0);
            bits.push_back(0);
            bits.push_back(0);
            offset++;
        }
    }
    return bits;
}

CutVertexIterator::CutVertexIterator(BasicGraph *graph)
    : g(graph), n(g->getOrder()), offs(0), edges(makeBits(g, &offs)) {}

void CutVertexIterator::setTreeEdge(uint u, uint v) {
    uint ui = offs[u] + edgeIndex(u, v), vi = offs[v] + edgeIndex(v, u);
    edges.insert(ui, edges.get(ui) | EDGE_BIT);
    edges.insert(vi, edges.get(vi) | EDGE_BIT);
    edges.insert(vi, edges.get(vi) | DIRECTION_BIT);
}

/**
 * Set the mark for edge {u,v}.
 */
void CutVertexIterator::setMark(uint u, uint v, uint8_t mark) {
    uint ui = offs[u] + edgeIndex(u, v), vi = offs[v] + edgeIndex(v, u);
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
        cc.emplace_back(a);
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
}

bool CutVertexIterator::more() { return cu < n; }

uint CutVertexIterator::next() {
    bool r = false;
    while (!r) {
        if (cu == cc[cci]) {
            cci++;
            uint num = 0;
            for (uint k = 0; k < n; k++) {
                if (isTreeEdge(cu, g->head(cu, k))) {
                    num++;
                }
                if (num > 1) {
                    r = true;
                }
            }
        } else {
            for (uint k = 0; k < g->getNodeDegree(cu); k++) {
                if (!isFullMarked(cu, g->head(cu, k))) {
                    r = true;
                }
            }
        }
        cu++;
        if (cu == n) return INVALID;
    }
    return cu - 1;
}
