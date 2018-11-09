#include "sealib/bcciterator.h"

using Sealib::BCCIterator;

void BCCIterator::init() {}

void BCCIterator::setOrigin(uint u, uint v) {}

bool BCCIterator::more() { return false; }

uint BCCIterator::next() { return 0; }

BCCIterator::BCCIterator(BasicGraph *graph) : g(graph), e(g) {}

BCCIterator::BCCIterator(BasicGraph *graph, EdgeMarker *edges)
    : g(graph), e(*edges) {}
