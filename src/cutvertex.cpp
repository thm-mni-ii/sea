#include "sealib/cutvertex.h"
#include <cmath>
#include "sealib/choicedictionary.h"
#include "sealib/staticspacestorage.h"

/** Edge data:
 *      EDMM
 *  E: edge type (0: back edge, 1: tree edge)
 *  D: "direction" (0: closer to root, 1: further away from root)
 *  M: marking (0: unmarked, 1: half marked, 2: full marked)
 */
#define EDGE_BIT 0x8       // 0b1000
#define DIRECTION_BIT 0x4  // 0b0100
#define MARKING_BITS 0x3   // 0b0011
#define SET_TREE(u, v)                          \
    uint ui = offs[(u)] + edgeIndex((u), (v)),  \
         vi = offs[(v)] + edgeIndex((v), (u));  \
    edges.insert(ui, edges.get(ui) | EDGE_BIT); \
    edges.insert(vi, edges.get(vi) | EDGE_BIT); \
    edges.insert(vi, edges.get(vi) | DIRECTION_BIT);
#define IS_TREE(u,v) ((getEdgeData((u), (v)) & EDGE_BIT) == 1)

using namespace Sealib;  // NOLINT

static std::vector<bool> makeBits(BasicGraph *g, std::vector<uint> *offs) {
    std::vector<bool> bits;
    uint offset = 0;
    for (uint u = 0; u < g->getOrder(); u++) {
        offs->emplace_back(offset);
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
    : g(graph), edges(makeBits(g, &offs)) {}

void CutVertexIterator::init() {
    uint n = g->getOrder();
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    std::vector<bool> bits;
    for (uint u = 0; u < n; u++) {
        bits.push_back(1);
        for (uint k = 0; k < ceil(log2(g->getNodeDegree(u) + 1)); k++) {
            bits.push_back(0);
        }
    }
    StaticSpaceStorage back(bits);
    for (uint a = 0; a < n; a++) {
        process_static(a, g, &color, &back, DFS_NOP_PROCESS,
                       [this, &color](uint u, uint v) {
                           if (color.get(v) == DFS_WHITE) {
                               SET_TREE(u, v);
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
            a, g, &color, &back,
            [this, &doScan](uint u) {
                if (doScan) {
                    doScan = false;
                    for (uint k = 0; k < g->getNodeDegree(u); k++) {
                        uint64_t e = getEdgeData(u, g->head(u, k));
                        if ((e & EDGE_BIT) == 0 && (e & DIRECTION_BIT) == 0) {
                            // {u,v} is a back edge and u is closer to root:
                            
                        }
                    }
                }
            },
            [this, &color, &doScan](uint u, uint v) {
                if (color.get(v) == DFS_WHITE && IS_TREE(u, v)) {
                    doScan = true;
                }
            },
            DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
    }
}

bool CutVertexIterator::more() { return false; }

uint CutVertexIterator::next() { return 0; }
