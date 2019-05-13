#include "sealib/iterator/bcciterator.h"

namespace Sealib {

BCCOutput::BCCOutput(UndirectedGraph const &graph)
    : BCCOutput(std::shared_ptr<EdgeMarker>(new EdgeMarker(graph))) {
    e->init();
}

BCCOutput::BCCOutput(std::shared_ptr<EdgeMarker> edges)
    : e(edges),
      g(e->getGraph()),
      n(g.getOrder()),
      color(n, 3),
      parent(e->getParent()) {}

void BCCOutput::traverse(uint64_t u0, Consumer onVertex, BiConsumer onEdge) {
    assert(!e->getCCs()[u0] &&
           "### u0 must not be a root vertex of the graph ###");
    color.insert(u0, DFS_GRAY);
    parent.insert(u0, g.deg(u0));
    onVertex(u0);
    outputBackEdges(u0, onEdge);
    uint64_t u = u0, k = 0;
    while (color.get(u0) != DFS_BLACK) {
        if (k < g.deg(u)) {
            uint64_t v = g.head(u, k);
            if (e->isTreeEdge(u, k) &&
                (e->isFullMarked(u, k) || !e->isParent(u, k)) &&
                color.get(v) == DFS_WHITE) {
                color.insert(v, DFS_GRAY);
                parent.insert(v, g.mate(u, k));
                onEdge(u, v);
                if (e->isFullMarked(u, k)) {
                    outputBackEdges(v, onEdge);
                    u = v;
                    k = 0;
                } else {
                    k = g.deg(u);  // retreat
                }
                onVertex(v);
            } else {
                k++;
            }
        } else {
            color.insert(u, DFS_BLACK);
            if (u != u0) {
                uint64_t pk = g.mate(u, parent.get(u)),
                         pu = g.head(u, parent.get(u));
                u = pu;
                k = pk + 1;
            }
        }
    }
}

void BCCOutput::outputBackEdges(uint64_t v, BiConsumer onEdge) {
    for (uint64_t l = 0; l < g.deg(v); l++) {
        if (e->isBackEdge(v, l) && !e->isParent(v, l)) {
            onEdge(g.head(v, l), v);
        }
    }
}

BCCIterator::BCCIterator(UndirectedGraph const &graph)
    : BCCIterator(std::shared_ptr<EdgeMarker>(new EdgeMarker(graph))) {
    e->init();
}

BCCIterator::BCCIterator(std::shared_ptr<EdgeMarker> edges)
    : e(edges), g(e->getGraph()), n(g.getOrder()), color(n, 3), parent(g) {}

void BCCIterator::init() {
    status = HAVE_NEXT;
    action = OUTPUT_BACK_EDGES;
    color.insert(node, DFS_GRAY);
}

void BCCIterator::init(uint64_t v) {
    startNode = node = latestNode = v;
    init();
}

bool BCCIterator::more() {
    switch (status) {
        case HAVE_NEXT:
            return true;
        case RETREAT:
            status = WAITING;
            edge = g.deg(node);
            [[clang::fallthrough]];
        case WAITING:
            while (node != startNode || color.get(node) != DFS_BLACK) {
                if (edge < g.deg(node)) {
                    if (e->isTreeEdge(node, edge) &&
                        (e->isFullMarked(node, edge) ||
                         !e->isParent(node, edge))) {
                        if (!e->isFullMarked(node, edge)) {
                            status = RETREAT;
                        }
                        uint64_t v = g.head(node, edge);
                        if (color.get(v) == DFS_WHITE) {
                            color.insert(v, DFS_GRAY);
                            parent.insert(v, g.mate(node, edge));
                            if (e->isFullMarked(node, edge)) {
                                action = OUTPUT_BACK_EDGES;
                            } else {
                                action = OUTPUT_VERTEX;
                            }
                            node = v;
                            edge = 0;
                            return true;
                        }
                    }
                    edge++;
                } else {
                    color.insert(node, DFS_BLACK);
                    if (node != startNode) {
                        uint64_t bk = parent.get(node), pu = g.head(node, bk),
                                 pk = g.mate(node, bk);
                        node = pu;
                        latestNode = node;
                        edge = pk + 1;
                    } else {
                        return false;
                    }
                }
            }
        default:
            return false;
    }
}

std::pair<uint64_t, uint64_t> BCCIterator::next() {
    std::pair<uint64_t, uint64_t> r(INVALID, INVALID);

    if (latestNode != node) {
        r = {latestNode, node};
        latestNode = node;
        if (status == RETREAT) gotRetreat = true;
        status = HAVE_NEXT;
    } else {
        switch (action) {
            case OUTPUT_BACK_EDGES:
                while (tmp < g.deg(node)) {
                    if (e->isBackEdge(node, tmp) && !e->isParent(node, tmp)) {
                        r = {g.head(node, tmp), node};
                        break;
                    }
                    tmp++;
                }
                if (r.first != INVALID) {
                    status = HAVE_NEXT;
                    tmp++;
                    break;
                } else {
                    tmp = 0;
                }
                [[clang::fallthrough]];
            case OUTPUT_VERTEX:
                r = {node, INVALID};
                if (gotRetreat) {
                    gotRetreat = false;
                    status = RETREAT;
                } else {
                    status = WAITING;
                }
                break;
            default:
                break;
        }
    }

    return r;
}

}  // namespace Sealib
