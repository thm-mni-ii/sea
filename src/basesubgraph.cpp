#include <sealib/basesubgraph.h>
#include <iostream>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::BaseSubGraph::~BaseSubGraph() = default;

Sealib::BaseSubGraph::BaseSubGraph(rgraph_t rGraph_) :
    SubGraph(initializeQSelect(rGraph_), initializePSelect(rGraph_)),
    rGraph(rGraph_) {}

Sealib::BaseSubGraph::bitset_t Sealib::BaseSubGraph::initializeQSelect(Sealib::BaseSubGraph::rgraph_t rGraph) {
    bitset_t bs(rGraph->getOrder());
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->getNodeDegree(i) > 0) {
            bs[i] = 1;
        }
    }
    return bs;
}

Sealib::BaseSubGraph::bitset_t Sealib::BaseSubGraph::initializePSelect(Sealib::BaseSubGraph::rgraph_t rGraph) {
    unsigned long dSum = 0;
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        dSum += rGraph->getNodeDegree(i);
    }

    bitset_t bs(dSum + 1);
    unsigned long deg = 0;
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->getNodeDegree(i) > 0) {
            deg += rGraph->getNodeDegree(i);
            bs[deg] = 1;
        }
    }
    return bs;
}

unsigned long Sealib::BaseSubGraph::head(unsigned long u, unsigned long k) const {
    return rGraph->head(static_cast<unsigned int>(u), static_cast<unsigned int>(k));
}

std::tuple<unsigned long, unsigned long> Sealib::BaseSubGraph::mate(unsigned long u, unsigned long k) const {
    return std::tuple<unsigned int, unsigned int>();
}

unsigned long Sealib::BaseSubGraph::order() const {
    return rGraph->getOrder();
}

unsigned long Sealib::BaseSubGraph::arcNumber(unsigned long j, unsigned long k) const {
    /**
     * p:001010101
     * q:1111
     */
    assert(degree(j) > 0);

    unsigned long qRank = qSelect.rank(j);
    unsigned long n = 0;
    if (qRank > 1) {
        n = pSelect.select(qRank - 1) - 1;
    }
    unsigned long arc = n + k;

    assert(pSelect.rank(n + 1) == pSelect.rank(arc));
    return arc;
}

std::tuple<unsigned long, unsigned long> Sealib::BaseSubGraph::inverseArcNumber(unsigned long r) const {
    assert(r > 0);
    unsigned long j = pSelect.rank(r);

    unsigned long a = qSelect.select(j);
    unsigned long b = pSelect.select(j - 1);
    return std::tuple<unsigned long, unsigned long>(a, r - b);
}

unsigned long Sealib::BaseSubGraph::degree(unsigned long v) const {
    unsigned long a = pSelect.select(qSelect.rank(v));
    unsigned long b = pSelect.select(qSelect.rank(v - 1));

    unsigned long deg = 0;
    if (a == b) {
        return 0;
    } else if (b == (unsigned long) -1) {
        return a - 1;
    } else {
        return a - b;
    }
}


unsigned long Sealib::BaseSubGraph::translateVertex(unsigned long u) const {
    return u;
}

unsigned long Sealib::BaseSubGraph::translateArc(unsigned long e) const {
    return e;
}
