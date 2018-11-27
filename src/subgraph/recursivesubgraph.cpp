#include "./recursivesubgraph.h"
#include <iostream>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::RecursiveSubGraph::~RecursiveSubGraph() = default;

Sealib::RecursiveSubGraph::RecursiveSubGraph(stack_t *stack_,
                                             uint64_t sidx_,
                                             uint64_t ridx_,
                                             const bitset_t &v,
                                             const bitset_t &a) :
    SubGraph(sidx_, ridx_, stack_),
    vSelect(initializeVSelect(v)), aSelect(initializeASelect(a)) {
    SubGraph *r = stack->clientList[stack_t::refs[ridx]];

    bitset_t p(rank_a(aSelect.size()));
    bitset_t q(rank_v(vSelect.size()));

    uint64_t degRSum = 0;
    uint64_t degSum = 0;
    for (uint64_t uR = 1; uR <= vSelect.size(); uR++) {  // iterate all vertices of G_r
        uint64_t u = phiInv(uR);
        uint64_t degR = r->degree(uR);
        if (u > 0) {  // if u exists in G_this
            uint64_t deg = 0;
            for (uint64_t i = degRSum; i < degRSum + degR; i++) {
                uint64_t pinv = psiInv(i + 1);
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
    pSelect = new RankSelect(p);
    qSelect = new RankSelect(q);
}

uint64_t Sealib::RecursiveSubGraph::head(uint64_t u, uint64_t k) const {
    SubGraph *r = stack->clientList[stack_t::refs[ridx]];
    return phiInv(r->head(r->gInv(psi(g(u, k)))));
}

std::tuple<uint64_t, uint64_t>
Sealib::RecursiveSubGraph::mate(uint64_t u, uint64_t k) const {
    SubGraph *r = stack->clientList[stack_t::refs[ridx]];
    return gInv(psiInv(r->g(r->mate(r->gInv(psi(g(u, k)))))));
}

uint64_t Sealib::RecursiveSubGraph::phi(uint64_t u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return select_v(u);
}

uint64_t Sealib::RecursiveSubGraph::phiInv(uint64_t u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    if (vSelect.getBitset()[u - 1]) {
        return rank_v(u);
    } else {
        return 0;
    }
}

uint64_t Sealib::RecursiveSubGraph::psi(uint64_t a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return select_a(a);
}

uint64_t Sealib::RecursiveSubGraph::psiInv(uint64_t a) const {
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

        for (uint64_t i = 0; i < v.size(); i++) {
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

        for (uint64_t i = 0; i < a.size(); i++) {
            if (a[i]) {
                aR[gL->select_a(i + 1) - 1] = 1;
            }
        }
        return aR;
    }
}
