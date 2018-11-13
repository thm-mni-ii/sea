#include "sealib/cutvertexiterator.h"
#include "sealib/choicedictionary.h"
#include "sealib/staticspacestorage.h"

using namespace Sealib;  // NOLINT

CutVertexIterator::CutVertexIterator(BasicGraph *graph)
    : g(graph), e(g), n(g->getOrder()), cc(n), cut(n), cutI(nullptr) {
    for (uint a = 0; a < n; a++) {
        // we have to initialize the choice dictionary "in reverse"
        cc.insert(a);
        cc.remove(a);
        // cut.insert(a);
        // cut.remove(a);
    }
}

void CutVertexIterator::init() {
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    StaticSpaceStorage parent(g);

    // identify connected components
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            cc.insert(a);
            process_static(a, g, &color, &parent, DFS_NOP_PROCESS,
                           DFS_NOP_EXPLORE, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
        }
    }

    e.identifyEdges(&color, &parent);

    for (uint a = 0; a < n; a++) {
        color.insert(a, DFS_WHITE);
    }
    e.markTreeEdges(&color, &parent);

    cutI = ChoiceDictionaryIterator(&cut);
    for (uint u = 0; u < n; u++) {
        if (cc.get(u)) {
            // u is root of a DFS tree
            uint num = 0;
            for (uint k = 0; k < n; k++) {
                if (e.isTreeEdge(u, k) && e.isParent(u, k)) {
                    num++;
                }
                if (num > 1) {
                    cut.insert(u);
                    break;
                }
            }
        } else {
            for (uint k = 0; k < g->getNodeDegree(u); k++) {
                if (e.isTreeEdge(u, k) && e.isParent(u, k) &&
                    !e.isFullMarked(u, k)) {
                    cut.insert(u);
                    break;
                }
            }
        }
    }
    // cutI = ChoiceDictionaryIterator(&cut);
}

bool CutVertexIterator::more() { return cutI.more(); }

uint CutVertexIterator::next() { return static_cast<uint>(cutI.next()); }

bool CutVertexIterator::isCutVertex(uint u) { return cut.get(u); }
