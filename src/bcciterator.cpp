#include "sealib/bcciterator.h"

using Sealib::BCCIterator;

BCCIterator::BCCIterator(EdgeMarker *edges)
    : g(edges->getGraph()), n(g->getOrder()), e(edges), color(n, 3), parent(g) {
    externalEdgeMarker = true;
}

BCCIterator::BCCIterator(BasicGraph *graph)
    : BCCIterator(new EdgeMarker(graph)) {
    externalEdgeMarker = false;
}

BCCIterator::~BCCIterator() {
    if (!externalEdgeMarker) {
        delete e;
    }
}

void BCCIterator::init() {
    if (!externalEdgeMarker) {
        for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
        e->identifyEdges(&color, &parent);

        for (uint a = 0; a < n; a++) {
            color.insert(a, DFS_WHITE);
        }
        e->markTreeEdges(&color, &parent);
    }
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
}

void BCCIterator::start(uint u, uint v) {
    startEdge = Pair(u, v);
    node = v;
    parent.insert(node, g->getNodeDegree(node));
    edge = 0;
    color.insert(node, DFS_GRAY);
}

// to do: output edge indices in addition to vertices

bool BCCIterator::more() {
    if (endOnNextStep) {
        return false;
    } else {
        while (true) {
            if (edge < g->getNodeDegree(node)) {
                if (e->isTreeEdge(node, edge) &&
                    (!e->isParent(node, edge) || e->isFullMarked(node, edge))) {
                    if (!e->isFullMarked(node, edge)) {
                        endOnNextStep = true;
                    }
                    uint v = g->head(node, edge);
                    if (color.get(v) == DFS_WHITE) {
                        color.insert(v, DFS_GRAY);
                        parent.insert(v, std::get<1>(g->mate(node, edge)));
                        node = v;
                        edge = 0;
                        return true;
                    }
                } else {
                    edge++;
                }
            } else {
                color.insert(node, DFS_BLACK);
                if (node != startEdge.tail()) {
                    std::tuple<uint, uint> p =
                        g->mate(node, static_cast<uint>(parent.get(node)));
                    uint pu = std::get<0>(p), pk = std::get<1>(p);
                    node = pu;
                    edge = pk + 1;
                } else {
                    return false;
                }
            }
        }
    }
}

uint BCCIterator::next() { return node; }
