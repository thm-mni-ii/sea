#include "sealib/bcciterator.h"

using Sealib::BCCIterator;

BCCIterator::BCCIterator(BasicGraph *graph)
    : g(graph), n(g->getOrder()), e(g) {}

BCCIterator::BCCIterator(BasicGraph *graph, EdgeMarker *edges)
    : g(graph), n(g->getOrder()), e(*edges) {}

void BCCIterator::init() {
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    StaticSpaceStorage parent(g);

    e.identifyEdges(&color, &parent);

    for (uint a = 0; a < n; a++) {
        color.insert(a, DFS_WHITE);
    }
    e.markTreeEdges(&color, &parent);
}

void BCCIterator::init(uint u, uint v) {
    startEdge = Pair(u, v);
    init();
}

bool BCCIterator::more() { return false; }

uint BCCIterator::next() { return 0; }
