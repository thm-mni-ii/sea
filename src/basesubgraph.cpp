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

Sealib::BaseSubGraph::bitset_t
Sealib::BaseSubGraph::initializeQSelect(Sealib::BaseSubGraph::rgraph_t rGraph) {
    bitset_t bs(rGraph->getOrder());
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->getNodeDegree(i) > 0) {
            bs[i] = 1;
        }
    }
    return bs;
}

Sealib::BaseSubGraph::bitset_t
Sealib::BaseSubGraph::initializePSelect(Sealib::BaseSubGraph::rgraph_t rGraph) {
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

unsigned long Sealib::BaseSubGraph::head(unsigned long u,
                                         unsigned long k) const {
    return rGraph->head(static_cast<unsigned int>(u), static_cast<unsigned int>(k));
}

std::tuple<unsigned long, unsigned long>
Sealib::BaseSubGraph::mate(unsigned long u, unsigned long k) const {
    return rGraph->mate(static_cast<unsigned int>(u), static_cast<unsigned int>(k));
}

unsigned long Sealib::BaseSubGraph::degree(unsigned long u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    unsigned long a = select_p(rank_q(u));  //  pSelect.select(qSelect.rank(v));
    if (a == (unsigned long) -1) {
        throw std::out_of_range("out of range on node idx v");
    }
    unsigned long b = select_p(rank_q(u - 1));  // pSelect.select(qSelect.rank(v - 1));

    if (a == b) {
        return 0;
    } else if (b == (unsigned long) -1) {
        return a - 1;
    } else {
        return a - b;
    }
}

unsigned long Sealib::BaseSubGraph::phi(unsigned long u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return u;
}
unsigned long Sealib::BaseSubGraph::psi(unsigned long a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return a;
}
unsigned long Sealib::BaseSubGraph::phiInv(unsigned long u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return u;
}
unsigned long Sealib::BaseSubGraph::psiInv(unsigned long a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return a;
}
