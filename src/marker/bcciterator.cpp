#include "sealib/iterator/bcciterator.h"

namespace Sealib {

BCCIterator::BCCIterator(EdgeMarker *edges)
    : g(edges->getGraph()), n(g->getOrder()), e(edges), color(n, 3), parent(g) {
    externalEdgeMarker = true;
}

BCCIterator::BCCIterator(UndirectedGraph const *graph)
    : g(graph),
      n(g->getOrder()),
      e(new EdgeMarker(graph)),
      color(n, 3),
      parent(g) {
    externalEdgeMarker = false;
}

BCCIterator::~BCCIterator() {
    if (!externalEdgeMarker) {
        delete e;
    }
}

void BCCIterator::init() {
    if (!externalEdgeMarker) {
        e->identifyEdges();
        e->markTreeEdges();
    }
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
}

void BCCIterator::start(uint u, uint v) {
    startEdge = std::pair<uint, uint>(u, v);
    node = v;
    edge = 0;
    firstNode = true;
    parent.insert(node, std::numeric_limits<uint>::max());
    color.insert(node, DFS_GRAY);
}

bool BCCIterator::more() {
    if (firstNode || oneMoreOutput || outputtingBackEdges) {
        return true;
    } else if (endOnNextNode) {
        return false;
    } else {
        while (true) {
            if (edge < g->deg(node)) {
                if (e->isTreeEdge(node, edge) &&
                    (!e->isParent(node, edge) || e->isFullMarked(node, edge))) {
                    if (!e->isFullMarked(node, edge)) {
                        endOnNextNode = true;
                    }
                    uint v = g->head(node, edge);
                    if (color.get(v) == DFS_WHITE) {
                        color.insert(v, DFS_GRAY);
                        parent.insert(v, g->mate(node, edge));
                        if (e->isFullMarked(node, edge)) {
                            outputtingBackEdges = true;
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
                    uint pk = g->mate(node, static_cast<uint>(parent.get(node))), pu=g->head(node,pk);
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
    std::pair<uint, uint> r;
    if (firstNode) {
        firstNode = false;
        latestNode = node;
        r = std::pair<uint, uint>(node, INVALID);
    } else {
        if (latestNode != node) {
            if (!outputtingBackEdges) oneMoreOutput = true;
            r = std::pair<uint, uint>(latestNode, node);
            latestNode = node;
        } else if (outputtingBackEdges) {
            bool haveEdge = false;
            while (k < g->deg(node)) {
                if (e->isBackEdge(node, k) && !e->isParent(node, k)) {
                    r = std::pair<uint, uint>(g->head(node, k), node);
                    k++;
                    haveEdge = true;
                    break;
                } else {
                    k++;
                }
            }
            if (k >= g->deg(node) && !haveEdge) {
                k = 0;
                outputtingBackEdges = false;
                return next();
            }
        } else {
            oneMoreOutput = false;
            r = std::pair<uint, uint>(node, INVALID);
            latestNode = node;
        }
    }
    return r;
}

}  // namespace Sealib
