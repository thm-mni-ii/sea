#include "sealib/iterator/cutvertexiterator.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/collection/staticspacestorage.h"

namespace Sealib {

CutVertexIterator::CutVertexIterator(EdgeMarker *edges)
    : g(edges->getGraph()), n(g->getOrder()), cc(n), cut(n), cutI(&cut) {
    externalEdgeMarker = true;
}

CutVertexIterator::CutVertexIterator(UndirectedGraph *graph)
    : g(graph),
      n(g->getOrder()),
      e(new EdgeMarker(graph)),
      cc(n),
      cut(n),
      cutI(&cut) {
    externalEdgeMarker = false;
}

CutVertexIterator::~CutVertexIterator() {
    if (!externalEdgeMarker) {
        delete e;
    }
}

void CutVertexIterator::init() {
    {
        CompactArray color(n, 3);
        for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
        StaticSpaceStorage parent(g);

        // identify connected components
        for (uint a = 0; a < n; a++) {
            if (color.get(a) == DFS_WHITE) {
                cc.insert(a);
                process_static(a, g, &color, &parent, DFS_NOP_PROCESS,
                               DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                               DFS_NOP_PROCESS);
            }
        }
    }

    e->identifyEdges();

    e->markTreeEdges();

    for (uint u = 0; u < n; u++) {
        if (cc.get(u)) {
            // u is root of a DFS tree
            uint num = 0;
            for (uint k = 0; k < g->getNodeDegree(u); k++) {
                if (e->isTreeEdge(u, k) && e->isParent(u, k)) {
                    num++;
                }
                if (num > 1) {
                    cut.insert(u);
                    break;
                }
            }
        } else {
            for (uint k = 0; k < g->getNodeDegree(u); k++) {
                if (e->isTreeEdge(u, k) && e->isParent(u, k) &&
                    !e->isFullMarked(u, k)) {
                    cut.insert(u);
                    break;
                }
            }
        }
    }
    cutI.init();
}

bool CutVertexIterator::more() { return cutI.more(); }

uint CutVertexIterator::next() { return static_cast<uint>(cutI.next()); }

bool CutVertexIterator::isCutVertex(uint u) { return cut.get(u); }

}  // namespace Sealib
