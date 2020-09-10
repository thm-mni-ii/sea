#include <sealib/collection/subgraphstack.h>
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

SubGraphStack::SubGraphStack(std::shared_ptr<UndirectedGraph> g_) : client_vector(),
                                                               currentRef(0),
                                                               tuned(INVALID),
                                                               tunedPhi0(nullptr),
                                                               tunedPsi0(nullptr),
                                                               tunedPhi(nullptr),
                                                               tunedPsi(nullptr) {
    client_vector.emplace_back(new BaseSubGraph(this, std::move(g_)));
}

SubGraphStack::~SubGraphStack() {
    for (SubGraph *g : client_vector) {
        delete g;
    }
    if (tunedPhi0) delete tunedPhi0;
    if (tunedPsi0) delete tunedPsi0;
    if (tunedPhi) delete tunedPhi;
    if (tunedPsi) delete tunedPsi;
}

void Sealib::SubGraphStack::push(const bitset_t &v,
                                 const bitset_t &a) {
    assert(currentRef+2 < SubGraphStack::refs.size());
    if (client_vector.size() - 1 == Sealib::SubGraphStack::refs[currentRef + 1]) {
        currentRef++;
    }
    SubGraph *g = new RecursiveSubGraph(this, client_vector.size(), currentRef, v, a);
    client_vector.emplace_back(g);
}

void Sealib::SubGraphStack::push(bitset_t &&v,
                                 bitset_t &&a) {
    assert(currentRef+2 < SubGraphStack::refs.size());
    if (client_vector.size() - 1 == Sealib::SubGraphStack::refs[currentRef + 1]) {
        currentRef++;
    }
    SubGraph *g =
        new RecursiveSubGraph(this, client_vector.size(), currentRef, std::move(v), std::move(a));
    client_vector.emplace_back(g);
}

void Sealib::SubGraphStack::pop() {
    currentRef = client_vector[client_vector.size() - 1]->getRidx();
    delete client_vector[client_vector.size() - 1];
    client_vector.pop_back();
}

uint64_t Sealib::SubGraphStack::order(uint64_t i) const {
    return client_vector[i]->order();
}

uint64_t Sealib::SubGraphStack::degree(uint64_t i,
                                            uint64_t u) const {
    return client_vector[i]->degree(u);
}

uint64_t Sealib::SubGraphStack::head(uint64_t i,
                                          uint64_t u,
                                          uint64_t k) const {
    return client_vector[i]->head(u, k);
}

std::tuple<uint64_t, uint64_t> Sealib::SubGraphStack::mate(uint64_t i,
                                                                     uint64_t u,
                                                                     uint64_t k) const {
    return client_vector[i]->mate(u, k);
}

uint64_t Sealib::SubGraphStack::g(uint64_t i,
                                       uint64_t u,
                                       uint64_t k) const {
    return client_vector[i]->g(u, k);
}

std::tuple<uint64_t, uint64_t>
Sealib::SubGraphStack::gInv(uint64_t i, uint64_t r) const {
    return client_vector[i]->gInv(r);
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
        uint64_t rIdx = client_vector[i]->getRidx();
        uint64_t uR = client_vector[i]->phi(u);

        while (rIdx != client_vector[j]->getRidx()) {
            uR = client_vector[refs[rIdx]]->phi(uR);
            rIdx = client_vector[refs[rIdx]]->getRidx();
        }
        return client_vector[j]->phiInv(uR);
    } else {
        uint64_t uR = client_vector[i]->phi(u);
        uint64_t rIdx = client_vector[i]->getRidx();

        while (rIdx != client_vector[j]->getRidx()) {
            rIdx++;
            assert(rIdx < SubGraphStack::refs.size());
            uR = client_vector[refs[rIdx]]->phiInv(uR);
            if (uR == 0) return 0;
        }
        return client_vector[j]->phiInv(uR);
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
        uint64_t rIdx = client_vector[i]->getRidx();
        uint64_t uR = client_vector[i]->psi(a);

        while (rIdx != client_vector[j]->getRidx()) {
            uR = client_vector[refs[rIdx]]->psi(uR);
            rIdx = client_vector[refs[rIdx]]->getRidx();
        }
        return client_vector[j]->psiInv(uR);
    } else {
        uint64_t uR = client_vector[i]->psi(a);
        uint64_t rIdx = client_vector[i]->getRidx();

        while (rIdx != client_vector[j]->getRidx()) {
            rIdx++;
            assert(rIdx < SubGraphStack::refs.size());
            uR = client_vector[refs[rIdx]]->psiInv(uR);
            if (uR == 0) return 0;
        }
        return client_vector[j]->psiInv(uR);
    }
}

void Sealib::SubGraphStack::push_vertex_induced(const bitset_t &v) {
    bitset_t a(gMax());
    for (uint64_t i = 0; i < a.size(); i++) {
        auto tmp = client_vector[client_vector.size() - 1]->gInv(i + 1);
        auto vertex = std::get<0>(tmp);
        auto k = std::get<1>(tmp);

        a.insert(i, v.get(vertex - 1) && v.get(head(vertex, k) - 1));
    }
    push(v, a);
}

void Sealib::SubGraphStack::push(const bitset_t &a) {
    bitset_t v(client_vector[client_vector.size() - 1]->order());
    for (uint64_t i = 0; i < a.size(); i++) {
        if (a[i]) {
            uint64_t vi = std::get<0>(
                client_vector[client_vector.size() - 1]->gInv(i + 1));
            v[vi - 1] = 1;
        }
    }
    push(v, a);
}

void Sealib::SubGraphStack::push(bitset_t &&a) {
    bitset_t v(client_vector[client_vector.size() - 1]->order());
    for (uint64_t i = 0; i < a.size(); i++) {
        if (a[i]) {
            uint64_t vi = std::get<0>(
                client_vector[client_vector.size() - 1]->gInv(i + 1));
            v[vi - 1] = 1;
        }
    }
    push(std::move(v), std::move(a));
}

uint64_t Sealib::SubGraphStack::gMax(uint64_t i) const {
    return client_vector[i]->gMax();
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
    bitset_t phi0bs(client_vector[0]->order());
    for (uint64_t u = 1; u <= client_vector[i]->order(); u++) {
        phi0bs[phi(i, u) - 1] = 1;
    }
    if (tunedPhi0) delete tunedPhi0;
    tunedPhi0 = new rankselect_t (std::move(phi0bs));
}

void Sealib::SubGraphStack::tunepsi0(uint64_t i) {
    bitset_t psi0bs(client_vector[0]->gMax());
    for (uint64_t u = 1; u <= client_vector[i]->gMax(); u++) {
        psi0bs[psi(i, u) - 1] = 1;
    }
    if (tunedPsi0) delete tunedPsi0;
    tunedPsi0 = new rankselect_t(std::move(psi0bs));
}

void Sealib::SubGraphStack::tunephi(uint64_t i) {
    bitset_t phibs(client_vector[i - 1]->order());
    for (uint64_t u = 1; u <= client_vector[i]->order(); u++) {
        phibs[phi(i, i - 1, u) - 1] = 1;
    }
    if (tunedPhi) delete tunedPhi;
    tunedPhi = new rankselect_t(std::move(phibs));
}

void Sealib::SubGraphStack::tunepsi(uint64_t i) {
    bitset_t psibs(client_vector[i - 1]->gMax());
    for (uint64_t u = 1; u <= client_vector[i]->gMax(); u++) {
        psibs[psi(i, i - 1, u) - 1] = 1;
    }
    if (tunedPsi) delete tunedPsi;
    tunedPsi = new rankselect_t(std::move(psibs));
}
