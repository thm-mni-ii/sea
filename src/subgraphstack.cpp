#include <sealib/subgraphstack.h>
#include <iostream>
#include "./subgraph.h"
#include "./recursivesubgraph.h"
#include "./basesubgraph.h"

using Sealib::SubGraph;
using Sealib::BaseSubGraph;
using Sealib::RecursiveSubGraph;
using Sealib::SubGraphStack;

unsigned long SubGraphStack::refs[6] = {0, 1, 3, 15, 65535, (unsigned long) -1};

SubGraphStack::SubGraphStack(std::shared_ptr<BasicGraph> g_) : clientList(),
                                                               currentRef(0),
                                                               tuned((unsigned long) -1),
                                                               tunedPhi0(nullptr),
                                                               tunedPsi0(nullptr),
                                                               tunedPhi(nullptr),
                                                               tunedPsi(nullptr) {
    clientList.emplace_back(new BaseSubGraph(this, g_));
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

void Sealib::SubGraphStack::push(const Sealib::Bitset<unsigned char> &v,
                                 const Sealib::Bitset<unsigned char> &a) {
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

unsigned long Sealib::SubGraphStack::order(unsigned long i) const {
    return clientList[i]->order();
}

unsigned long Sealib::SubGraphStack::degree(unsigned long i,
                                            unsigned long u) const {
    return clientList[i]->degree(u);
}

unsigned long Sealib::SubGraphStack::head(unsigned long i,
                                          unsigned long u,
                                          unsigned long k) const {
    return clientList[i]->head(u, k);
}

std::tuple<unsigned long, unsigned long> Sealib::SubGraphStack::mate(unsigned long i,
                                                                     unsigned long u,
                                                                     unsigned long k) const {
    return clientList[i]->mate(u, k);
}

unsigned long Sealib::SubGraphStack::g(unsigned long i,
                                       unsigned long u,
                                       unsigned long k) const {
    return clientList[i]->g(u, k);
}

std::tuple<unsigned long, unsigned long>
Sealib::SubGraphStack::gInv(unsigned long i, unsigned long r) const {
    return clientList[i]->gInv(r);
}

unsigned long Sealib::SubGraphStack::phi(unsigned long i,
                                         unsigned long j,
                                         unsigned long u) const {
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
    } else if (i == j) {
        return u;
    }
    /** tuned end */

    if (i > j) {
        unsigned long rIdx = clientList[i]->getRidx();
        unsigned long uR = clientList[i]->phi(u);

        while (rIdx != clientList[j]->getRidx()) {
            uR = clientList[refs[rIdx]]->phi(uR);
            rIdx = clientList[refs[rIdx]]->getRidx();
        }
        return clientList[j]->phiInv(uR);
    } else {
        unsigned long uR = clientList[i]->phi(u);
        unsigned long rIdx = clientList[i]->getRidx();

        while (rIdx != clientList[j]->getRidx()) {
            rIdx++;
            uR = clientList[refs[rIdx]]->phiInv(uR);
            if (uR == 0) return 0;
        }
        return clientList[j]->phiInv(uR);
    }
}

unsigned long Sealib::SubGraphStack::psi(unsigned long i,
                                         unsigned long j,
                                         unsigned long a) const {
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
    } else if (i == j) {
        return a;
    }
    /** tuned end */

    if (i > j) {
        unsigned long rIdx = clientList[i]->getRidx();
        unsigned long uR = clientList[i]->psi(a);

        while (rIdx != clientList[j]->getRidx()) {
            uR = clientList[refs[rIdx]]->psi(uR);
            rIdx = clientList[refs[rIdx]]->getRidx();
        }
        return clientList[j]->psiInv(uR);
    } else {
        unsigned long uR = clientList[i]->psi(a);
        unsigned long rIdx = clientList[i]->getRidx();

        while (rIdx != clientList[j]->getRidx()) {
            rIdx++;
            uR = clientList[refs[rIdx]]->psiInv(uR);
            if (uR == 0) return 0;
        }
        return clientList[j]->psiInv(uR);
    }
}

void Sealib::SubGraphStack::push(const Sealib::Bitset<unsigned char> &a) {
    Sealib::Bitset<unsigned char> v(clientList[clientList.size() - 1]->order());
    for (unsigned long i = 0; i < a.size(); i++) {
        if (a[i]) {
            unsigned long vi = std::get<0>(clientList[clientList.size() - 1]->gInv(i + 1));
            v[vi - 1] = 1;
        }
    }
    push(v, a);
}

unsigned long Sealib::SubGraphStack::gMax(unsigned long i) const {
    return clientList[i]->gMax();
}

void Sealib::SubGraphStack::tune(unsigned long i) {
    if (i == 0)
        return;  // G_0 is always tuned
    tunephi0(i);
    tunepsi0(i);
    tunephi(i);
    tunepsi(i);
}

void Sealib::SubGraphStack::tunephi0(unsigned long i) {
    Sealib::Bitset<unsigned char> phi0bs(clientList[0]->order());
    for (unsigned long u = 1; u <= clientList[i]->order(); u++) {
        phi0bs[phi(i, u) - 1] = 1;
    }
    if (tunedPhi0) delete tunedPhi0;
    tunedPhi0 = new RankSelect<>(phi0bs);
}

void Sealib::SubGraphStack::tunepsi0(unsigned long i) {
    Sealib::Bitset<unsigned char> psi0bs(clientList[0]->gMax());
    for (unsigned long u = 1; u <= clientList[i]->gMax(); u++) {
        psi0bs[psi(i, u) - 1] = 1;
    }
    if (tunedPsi0) delete tunedPsi0;
    tunedPsi0 = new RankSelect<>(psi0bs);
}

void Sealib::SubGraphStack::tunephi(unsigned long i) {
    Sealib::Bitset<unsigned char> phibs(clientList[i - 1]->order());
    for (unsigned long u = 1; u <= clientList[i]->order(); u++) {
        phibs[phi(i, i - 1, u) - 1] = 1;
    }
    if (tunedPhi) delete tunedPhi;
    tunedPhi = new RankSelect<>(phibs);
}

void Sealib::SubGraphStack::tunepsi(unsigned long i) {
    Sealib::Bitset<unsigned char> psibs(clientList[i - 1]->gMax());
    for (unsigned long u = 1; u <= clientList[i]->gMax(); u++) {
        psibs[psi(i, i - 1, u) - 1] = 1;
    }
    if (tunedPsi) delete tunedPsi;
    tunedPsi = new RankSelect<>(psibs);
}
