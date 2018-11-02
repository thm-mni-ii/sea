#include "./basesubgraph.h"
#include <iostream>
#include <utility>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::BaseSubGraph::~BaseSubGraph() = default;

Sealib::BaseSubGraph::BaseSubGraph(stack_t *stack_, rgraph_t rGraph_) :
    SubGraph(0, 0, stack_),
    rGraph(std::move(rGraph_)) {
    uint64_t dSum = 0;
    bitset_t q(rGraph->getOrder());
    for (uint32_t i = 0; i < rGraph->getOrder(); i++) {
        uint64_t deg = rGraph->getNodeDegree(i);
        dSum += deg;
        if (deg > 0) {
            q[i] = 1;
        }
    }

    bitset_t p(dSum);
    uint64_t deg = 0;
    for (uint32_t i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->getNodeDegree(i) > 0) {
            deg += rGraph->getNodeDegree(i);
            p[deg - 1] = 1;
        }
    }
    qSelect = new RankSelect(q);
    pSelect = new RankSelect(p);
}

uint64_t Sealib::BaseSubGraph::head(uint64_t u,
                                         uint64_t k) const {
    return rGraph->head(static_cast<uint32_t>(u - 1), static_cast<uint32_t>(k - 1)) + 1;
}

std::tuple<uint64_t, uint64_t>
Sealib::BaseSubGraph::mate(uint64_t u, uint64_t k) const {
    std::tuple<uint64_t, uint64_t>
        mate = rGraph->mate(
            static_cast<uint32_t>(u - 1),
            static_cast<uint32_t>(k - 1));
    return
        std::tuple<uint64_t, uint64_t>(
            std::get<0>(mate) + 1,
            std::get<1>(mate) + 1);
}

uint64_t Sealib::BaseSubGraph::phi(uint64_t u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return u;
}
uint64_t Sealib::BaseSubGraph::psi(uint64_t a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return a;
}
uint64_t Sealib::BaseSubGraph::phiInv(uint64_t u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return u;
}
uint64_t Sealib::BaseSubGraph::psiInv(uint64_t a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return a;
}
