#include <sealib/basesubgraph.h>

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

    bitset_t bs(dSum);
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        bs[rGraph->getNodeDegree(i)] = 1;
    }
    return bs;
}

unsigned long Sealib::BaseSubGraph::degree(unsigned long v) const {
    return rGraph->getNodeDegree(static_cast<unsigned int>(v));
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

unsigned long Sealib::BaseSubGraph::arcNumber(unsigned long j, unsigned long k) {
    return pSelect.select(qSelect.rank(j) - 1) + k;
}

std::tuple<unsigned long, unsigned long> Sealib::BaseSubGraph::inverseArcNumber(unsigned long r) {
    unsigned long j = pSelect.rank(r - 1) + 1;
    return std::tuple<unsigned long, unsigned long>(qSelect.select(j), r - pSelect.select(j-1));
}

unsigned long Sealib::BaseSubGraph::translateVertex(unsigned long u) {
    return u;
}

unsigned long Sealib::BaseSubGraph::translateArc(unsigned long e) {
    return e;
}
