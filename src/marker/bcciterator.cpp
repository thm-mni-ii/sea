#include "sealib/iterator/bcciterator.h"

namespace Sealib {

BCCIterator::BCCIterator(UndirectedGraph const *graph)
    : BCCIterator(std::shared_ptr<EdgeMarker>(new EdgeMarker(graph))) {
    e->init();
}

BCCIterator::BCCIterator(std::shared_ptr<EdgeMarker> edges)
    : e(edges), g(e->getGraph()), n(g->getOrder()), color(n, 3), parent(g) {}

void BCCIterator::init() {
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
}

void BCCIterator::start(uint u, uint v) {
    startEdge = std::pair<uint, uint>(u, v);
    node = v;
    latestNode = node;
    edge = 0;
    status = HAVE_NEXT;
    action = OUTPUT_VERTEX;
    parent.insert(node, INVALID);
    color.insert(node, DFS_GRAY);
}

bool BCCIterator::more() {
    if (status == HAVE_NEXT) {
        return true;
    } else if (status == END) {
        return false;
    } else {
        while (true) {
            if (edge < g->deg(node)) {
                if (e->isTreeEdge(node, edge) &&
                    (e->isFullMarked(node, edge) || !e->isParent(node, edge))) {
                    if (!e->isFullMarked(node, edge)) {
                        status = END;
                    }
                    uint v = g->head(node, edge);
                    if (color.get(v) == DFS_WHITE) {
                        color.insert(v, DFS_GRAY);
                        parent.insert(v, g->mate(node, edge));
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
                if (node != startEdge.second) {
                    uint pk =
                             g->mate(node, static_cast<uint>(parent.get(node))),
                         pu = g->head(node, pk);
                    node = pu;
                    edge = pk + 1;
                } else {
                    return false;
                }
            }
        }
    }
}

std::pair<uint, uint> BCCIterator::next() {
    static uint k = 0;
    std::pair<uint, uint> r(INVALID, INVALID);

    if (latestNode != node) {
        r = std::pair<uint, uint>(latestNode, node);
        latestNode = node;
        status = HAVE_NEXT;
    } else {
        switch (action) {
            case OUTPUT_BACK_EDGES:
                while (k < g->deg(node)) {
                    if (e->isBackEdge(node, k) && !e->isParent(node, k)) {
                        r = std::pair<uint, uint>(g->head(node, k), node);
                        break;
                    }
                    k++;
                }
                if (r.first != INVALID) {
                    status = HAVE_NEXT;
                    k++;
                    break;
                } else {
                    k = 0;
                    // fall through to OUTPUT_VERTEX
                }
            case OUTPUT_VERTEX:
                r = std::pair<uint, uint>(node, INVALID);
                status = WAITING;
                break;
            case NO_ACTION:
                assert(false && "no action specified!");
                break;
        }
    }

    return r;
}

}  // namespace Sealib
