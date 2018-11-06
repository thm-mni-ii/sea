#include "src/basesubgraph.h"
#include <iostream>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::BaseSubGraph::~BaseSubGraph() = default;

Sealib::BaseSubGraph::BaseSubGraph(stack_t *stack_, rgraph_t rGraph_) :
    SubGraph(0, 0, stack_),
    rGraph(rGraph_) {
    unsigned long dSum = 0;
    bitset_t q(rGraph->getOrder());
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        unsigned long deg = rGraph->getNodeDegree(i);
        dSum += deg;
        if (deg > 0) {
            q[i] = 1;
        }
    }

    bitset_t p(dSum);
    unsigned long deg = 0;
    for (unsigned int i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->getNodeDegree(i) > 0) {
            deg += rGraph->getNodeDegree(i);
            p[deg - 1] = 1;
        }
    }
    qSelect = new RankSelect<>(q);
    pSelect = new RankSelect<>(p);
}

unsigned long Sealib::BaseSubGraph::head(unsigned long u,
                                         unsigned long k) const {
    return rGraph->head(static_cast<unsigned int>(u - 1), static_cast<unsigned int>(k - 1)) + 1;
}

std::tuple<unsigned long, unsigned long>
Sealib::BaseSubGraph::mate(unsigned long u, unsigned long k) const {
    std::tuple<unsigned long, unsigned long>
        mate = rGraph->mate(
            static_cast<unsigned int>(u - 1),
            static_cast<unsigned int>(k - 1));
    return
        std::tuple<unsigned long, unsigned long>(
            std::get<0>(mate) + 1,
            std::get<1>(mate) + 1);
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
