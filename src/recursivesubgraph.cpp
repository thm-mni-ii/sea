#include <sealib/recursivesubgraph.h>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::RecursiveSubGraph::~RecursiveSubGraph() {}

Sealib::RecursiveSubGraph::RecursiveSubGraph(Sealib::SubGraph *rGraph_,
                                             const Sealib::Bitset<unsigned char> &v,
                                             const Sealib::Bitset<unsigned char> &e) : SubGraph(v, e) {}

Sealib::SubGraph::bitset_t Sealib::RecursiveSubGraph::initializeQSelect() {
    return Sealib::SubGraph::bitset_t();
}

Sealib::SubGraph::bitset_t Sealib::RecursiveSubGraph::initializePSelect() {
    return Sealib::SubGraph::bitset_t();
}

unsigned long Sealib::RecursiveSubGraph::degree(unsigned long u) const {
    return 0;
}

unsigned long Sealib::RecursiveSubGraph::head(unsigned long u, unsigned long k) const {
    return translateVertex(rGraph->head(rGraph->gInv(translateArc(g(u, k)))));
}

std::tuple<unsigned long, unsigned long>
Sealib::RecursiveSubGraph::mate(unsigned long u, unsigned long k) const {
    return std::tuple<unsigned long, unsigned long>();
}

unsigned long Sealib::RecursiveSubGraph::phi(unsigned long u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return select_v(u);
}

unsigned long Sealib::RecursiveSubGraph::phiInv(unsigned long u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    if (vSelect.getBitset()[u]) {
        return rank_v(u);
    } else {
        return 0;
    }
}

unsigned long Sealib::RecursiveSubGraph::psi(unsigned long a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return select_a(a);
}

unsigned long Sealib::RecursiveSubGraph::psiInv(unsigned long a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return rank_a(a);
}
