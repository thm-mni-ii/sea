#include "sealib/iterator/bcciterator.h"

namespace Sealib {

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
            while (true) {
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
