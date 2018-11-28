#include <sealib/subgraphstack.h>
#include <iostream>
#include <utility>
#include "./subgraph.h"
#include "./recursivesubgraph.h"
#include "./basesubgraph.h"

using Sealib::SubGraph;
using Sealib::BaseSubGraph;
using Sealib::RecursiveSubGraph;
using Sealib::SubGraphStack;

std::vector<uint64_t> SubGraphStack::refs = {0, 1, 3, 15, 65535, static_cast<uint64_t>(-1)};

SubGraphStack::SubGraphStack(std::shared_ptr<UndirectedGraph> g_) : clientList(),
                                                               currentRef(0),
                                                               tuned((uint64_t) -1),
                                                               tunedPhi0(nullptr),
                                                               tunedPsi0(nullptr),
                                                               tunedPhi(nullptr),
                                                               tunedPsi(nullptr) {
    clientList.emplace_back(new BaseSubGraph(this, std::move(g_)));
}

SubGraphStack::~SubGraphStack() {
    for (SubGraph *g : clientList) {
        delete g;
    }
    if (tunedPhi0) delete tunedPhi0;
    if (tunedPsi0) delete tunedPsi0;
    if (tunedPhi) delete tunedPhi;
    if (tunedPsi) delete tunedPsi;
}

void Sealib::SubGraphStack::push(const Sealib::Bitset<uint8_t> &v,
                                 const Sealib::Bitset<uint8_t> &a) {
    assert(currentRef+2 < SubGraphStack::refs.size());
    if (clientList.size() - 1 == Sealib::SubGraphStack::refs[currentRef + 1]) {
        currentRef++;
    }
    SubGraph *g = new RecursiveSubGraph(this, clientList.size(), currentRef, v, a);
    clientList.emplace_back(g);
}

void Sealib::SubGraphStack::pop() {
    currentRef = clientList[clientList.size() - 1]->getRidx();
    delete clientList[clientList.size() - 1];
    clientList.pop_back();
}

uint64_t Sealib::SubGraphStack::order(uint64_t i) const {
    return clientList[i]->order();
}

uint64_t Sealib::SubGraphStack::degree(uint64_t i,
                                            uint64_t u) const {
    return clientList[i]->degree(u);
}

uint64_t Sealib::SubGraphStack::head(uint64_t i,
                                          uint64_t u,
                                          uint64_t k) const {
    return clientList[i]->head(u, k);
}

std::tuple<uint64_t, uint64_t> Sealib::SubGraphStack::mate(uint64_t i,
                                                                     uint64_t u,
                                                                     uint64_t k) const {
    return clientList[i]->mate(u, k);
}

uint64_t Sealib::SubGraphStack::g(uint64_t i,
                                       uint64_t u,
                                       uint64_t k) const {
    return clientList[i]->g(u, k);
}

std::tuple<uint64_t, uint64_t>
Sealib::SubGraphStack::gInv(uint64_t i, uint64_t r) const {
    return clientList[i]->gInv(r);
}

uint64_t Sealib::SubGraphStack::phi(uint64_t i,
                                         uint64_t j,
                                         uint64_t u) const {
    if (i == j) return u;

    /** tuned check */
    if (i == tuned && j == 0) {
        return tunedPhi0->select(u);
    } else if (j == tuned && i == 0) {
        return tunedPhi0->getBitset()[u - 1] ? tunedPhi0->rank(u) : 0;
    } else if (i > 0 && i == tuned && j == i - 1) {
        return tunedPhi->select(u);
    } else if (j > 0 && j == tuned && i == j - 1) {
        return tunedPhi->getBitset()[u - 1] ? tunedPhi->rank(u) : 0;
    }
    /** tuned end */

    if (i > j) {
        uint64_t rIdx = clientList[i]->getRidx();
        uint64_t uR = clientList[i]->phi(u);

        while (rIdx != clientList[j]->getRidx()) {
            uR = clientList[refs[rIdx]]->phi(uR);
            rIdx = clientList[refs[rIdx]]->getRidx();
        }
        return clientList[j]->phiInv(uR);
    } else {
        uint64_t uR = clientList[i]->phi(u);
        uint64_t rIdx = clientList[i]->getRidx();

        while (rIdx != clientList[j]->getRidx()) {
            rIdx++;
            assert(rIdx < SubGraphStack::refs.size());
            uR = clientList[refs[rIdx]]->phiInv(uR);
            if (uR == 0) return 0;
        }
        return clientList[j]->phiInv(uR);
    }
}

uint64_t Sealib::SubGraphStack::psi(uint64_t i,
                                         uint64_t j,
                                         uint64_t a) const {
    if (i == j) {
        return a;
    }

    /** tuned check */
    if (i == tuned && j == 0) {
        return tunedPsi0->select(a);
    } else if (j == tuned && i == 0) {
        return tunedPsi0->getBitset()[a - 1] ? tunedPsi0->rank(a) : 0;
    } else if (i > 0 && i == tuned && j == i - 1) {
        return tunedPsi->select(a);
    } else if (j > 0 && j == tuned && i == j - 1) {
        return tunedPsi->getBitset()[a - 1] ? tunedPsi->rank(a) : 0;
    }
    /** tuned end */

    if (i > j) {
        uint64_t rIdx = clientList[i]->getRidx();
        uint64_t uR = clientList[i]->psi(a);

        while (rIdx != clientList[j]->getRidx()) {
            uR = clientList[refs[rIdx]]->psi(uR);
            rIdx = clientList[refs[rIdx]]->getRidx();
        }
        return clientList[j]->psiInv(uR);
    } else {
        uint64_t uR = clientList[i]->psi(a);
        uint64_t rIdx = clientList[i]->getRidx();

        while (rIdx != clientList[j]->getRidx()) {
            rIdx++;
            assert(rIdx < SubGraphStack::refs.size());
            uR = clientList[refs[rIdx]]->psiInv(uR);
            if (uR == 0) return 0;
        }
        return clientList[j]->psiInv(uR);
    }
}

void Sealib::SubGraphStack::push(const Sealib::Bitset<uint8_t> &a) {
    Sealib::Bitset<uint8_t> v(clientList[clientList.size() - 1]->order());
    for (uint64_t i = 0; i < a.size(); i++) {
        if (a[i]) {
            uint64_t vi = std::get<0>(clientList[clientList.size() - 1]->gInv(i + 1));
            v[vi - 1] = 1;
        }
    }
    push(v, a);
}

uint64_t Sealib::SubGraphStack::gMax(uint64_t i) const {
    return clientList[i]->gMax();
}

void Sealib::SubGraphStack::tune(uint64_t i) {
    if (i == 0)
        return;  // G_0 is always tuned
    tunephi0(i);
    tunepsi0(i);
    tunephi(i);
    tunepsi(i);
}

void Sealib::SubGraphStack::tunephi0(uint64_t i) {
    Sealib::Bitset<uint8_t> phi0bs(clientList[0]->order());
    for (uint64_t u = 1; u <= clientList[i]->order(); u++) {
        phi0bs[phi(i, u) - 1] = 1;
    }
    if (tunedPhi0) delete tunedPhi0;
    tunedPhi0 = new RankSelect(phi0bs);
}

void Sealib::SubGraphStack::tunepsi0(uint64_t i) {
    Sealib::Bitset<uint8_t> psi0bs(clientList[0]->gMax());
    for (uint64_t u = 1; u <= clientList[i]->gMax(); u++) {
        psi0bs[psi(i, u) - 1] = 1;
    }
    if (tunedPsi0) delete tunedPsi0;
    tunedPsi0 = new RankSelect(psi0bs);
}

void Sealib::SubGraphStack::tunephi(uint64_t i) {
    Sealib::Bitset<uint8_t> phibs(clientList[i - 1]->order());
    for (uint64_t u = 1; u <= clientList[i]->order(); u++) {
        phibs[phi(i, i - 1, u) - 1] = 1;
    }
    if (tunedPhi) delete tunedPhi;
    tunedPhi = new RankSelect(phibs);
}

void Sealib::SubGraphStack::tunepsi(uint64_t i) {
    Sealib::Bitset<uint8_t> psibs(clientList[i - 1]->gMax());
    for (uint64_t u = 1; u <= clientList[i]->gMax(); u++) {
        psibs[psi(i, i - 1, u) - 1] = 1;
    }
    if (tunedPsi) delete tunedPsi;
    tunedPsi = new RankSelect(psibs);
}
