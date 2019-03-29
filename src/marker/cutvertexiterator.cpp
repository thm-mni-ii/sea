#include "sealib/iterator/cutvertexiterator.h"
#include "sealib/collection/staticspacestorage.h"
#include "sealib/dictionary/choicedictionary.h"

namespace Sealib {

CutVertexIterator::CutVertexIterator(UndirectedGraph const &graph)
    : CutVertexIterator(std::shared_ptr<EdgeMarker>(new EdgeMarker(graph))) {
    e->init();
}

CutVertexIterator::CutVertexIterator(std::shared_ptr<EdgeMarker> edges)
    : e(edges), g(e->getGraph()), n(g.getOrder()), cc(n), cut(n), cutI(cut) {}

void CutVertexIterator::findCCs() {
    CompactArray color(n, 3);
    StaticSpaceStorage parent(g);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            cc.insert(a);
            DFS::visit_nplusm(a, g, &color, &parent, DFS_NOP_PROCESS,
                              DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                              DFS_NOP_PROCESS);
        }
    }
}

void CutVertexIterator::init() {
    findCCs();

    for (uint64_t u = 0; u < n; u++) {
        if (cc.get(u)) {
            // u is root of a DFS tree
            uint64_t num = 0;
            for (uint64_t k = 0; k < g.deg(u); k++) {
                if (e->isTreeEdge(u, k) && e->isParent(u, k)) {
                    num++;
                }
                if (num > 1) {
                    cut.insert(u);
                    break;
                }
            }
        } else {
            for (uint64_t k = 0; k < g.deg(u); k++) {
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

uint64_t CutVertexIterator::next() { return cutI.next(); }

bool CutVertexIterator::isCutVertex(uint64_t u) { return cut.get(u); }

}  // namespace Sealib
