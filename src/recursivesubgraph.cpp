#include "src/recursivesubgraph.h"
#include <iostream>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::RecursiveSubGraph::~RecursiveSubGraph() = default;

Sealib::RecursiveSubGraph::RecursiveSubGraph(stack_t *stack_,
                                             unsigned long sidx_,
                                             unsigned long ridx_,
                                             const bitset_t &v,
                                             const bitset_t &a) :
    SubGraph(sidx_, ridx_, stack_),
    vSelect(initializeVSelect(v)), aSelect(initializeASelect(a)) {
    SubGraph *r = stack->clientList[stack_t::refs[ridx]];

    bitset_t p(rank_a(aSelect.size()));
    bitset_t q(rank_v(vSelect.size()));

    unsigned long degRSum = 0;
    unsigned long degSum = 0;
    for (unsigned long uR = 1; uR <= vSelect.size(); uR++) {  // iterate all vertices of G_r
        unsigned long u = phiInv(uR);
        unsigned long degR = r->degree(uR);
        if (u > 0) {  // if u exists in G_this
            unsigned long deg = 0;
            for (unsigned long i = degRSum; i < degRSum + degR; i++) {
                unsigned long pinv = psiInv(i + 1);
                deg += pinv == 0 ? 0 : 1;
            }
            degSum += deg;
            if (deg > 0) {
                q[u - 1] = 1;
                p[degSum - 1] = 1;
            }
        }
        degRSum += degR;
    }
    pSelect = new RankSelect<>(p);
    qSelect = new RankSelect<>(q);
}

unsigned long Sealib::RecursiveSubGraph::head(unsigned long u, unsigned long k) const {
    SubGraph *r = stack->clientList[stack_t::refs[ridx]];
    return phiInv(r->head(r->gInv(psi(g(u, k)))));
}

std::tuple<unsigned long, unsigned long>
Sealib::RecursiveSubGraph::mate(unsigned long u, unsigned long k) const {
    SubGraph *r = stack->clientList[stack_t::refs[ridx]];
    return gInv(psiInv(r->g(r->mate(r->gInv(psi(g(u, k)))))));
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
    if (vSelect.getBitset()[u - 1]) {
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
    if (aSelect.getBitset()[a - 1]) {
        return rank_a(a);
    } else {
        return 0;
    }
}

Sealib::SubGraph::bitset_t Sealib::RecursiveSubGraph::initializeVSelect(const bitset_t &v) {
    if (stack_t::refs[ridx] == sidx - 1) {
        return v;
    } else {
        auto *gL = reinterpret_cast<RecursiveSubGraph *>(stack->clientList[sidx - 1]);
        bitset_t vR(gL->vSelect.size());

        for (unsigned long i = 0; i < v.size(); i++) {
            if (v[i]) {
                vR[gL->select_v(i + 1) - 1] = 1;
            }
        }
        return vR;
    }
}

Sealib::SubGraph::bitset_t Sealib::RecursiveSubGraph::initializeASelect(const bitset_t &a) {
    if (stack_t::refs[ridx] == sidx - 1) {
        return a;
    } else {
        auto *gL = reinterpret_cast<RecursiveSubGraph *>(stack->clientList[sidx - 1]);
        bitset_t aR(gL->aSelect.size());

        for (unsigned long i = 0; i < a.size(); i++) {
            if (a[i]) {
                aR[gL->select_a(i + 1) - 1] = 1;
            }
        }
        return aR;
    }
}
