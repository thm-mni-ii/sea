#include <sealib/subgraphstack.h>
#include <sealib/subgraph.h>
#include <sealib/recursivesubgraph.h>
#include <sealib/basesubgraph.h>

using Sealib::SubGraph;
using Sealib::BaseSubGraph;
using Sealib::RecursiveSubGraph;
using Sealib::SubGraphStack;

SubGraphStack::SubGraphStack(std::shared_ptr<BasicGraph> g_) : clientList() {
    clientList.emplace_back(new BaseSubGraph(this, g_));
}

SubGraphStack::~SubGraphStack() {
    for (SubGraph *g : clientList) {
        delete g;
    }
}
void Sealib::SubGraphStack::push(const Sealib::Bitset<unsigned char> &v, const Sealib::Bitset<unsigned char> &e) {
    if(clientList.size() > refs[currentRef]) {
        currentRef++;
    }
    SubGraph *g = new RecursiveSubGraph(this, clientList.size(), currentRef, v, e);
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

unsigned long Sealib::SubGraphStack::degree(unsigned long i, unsigned long u) const {
    return clientList[i]->degree(i);
}

unsigned long Sealib::SubGraphStack::head(unsigned long i, unsigned long u, unsigned long k) const {
    return clientList[i]->head(u, k);
}

std::tuple<unsigned long, unsigned long> Sealib::SubGraphStack::mate(unsigned long i,
                                                                     unsigned long u,
                                                                     unsigned long k) const {
    return clientList[i]->mate(u, k);
}

unsigned long Sealib::SubGraphStack::g(unsigned long i, unsigned long u, unsigned long k) {
    return clientList[i]->g(u, k);
}

std::tuple<unsigned long, unsigned long> Sealib::SubGraphStack::gInv(unsigned long i, unsigned long r) {
    return clientList[i]->gInv(r);
}

unsigned long Sealib::SubGraphStack::phi(unsigned long i, unsigned long j, unsigned long u) {
    if (i == j) {
        return u;
    } else if (i > j) {
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

        while(rIdx != clientList[j]->getRidx()) {
            rIdx++;
            uR = clientList[refs[rIdx]]->phiInv(uR);
            if(uR == 0) return 0;
        }
        return clientList[j]->phiInv(uR);
    }
}

unsigned long Sealib::SubGraphStack::psi(unsigned long i, unsigned long j, unsigned long a) {
    if (i == j) {
        return u;
    } else if (i > j) {
        unsigned long rIdx = clientList[i]->getRidx();
        unsigned long uR = clientList[i]->psi(u);

        while (rIdx != clientList[j]->getRidx()) {
            uR = clientList[refs[rIdx]]->psi(uR);
            rIdx = clientList[refs[rIdx]]->getRidx();
        }
        return clientList[j]->psiInv(uR);
    } else {
        unsigned long uR = clientList[i]->psi(u);
        unsigned long rIdx = clientList[i]->getRidx();

        while(rIdx != clientList[j]->getRidx()) {
            rIdx++;
            uR = clientList[refs[rIdx]]->psiInv(uR);
            if(uR == 0) return 0;
        }
        return clientList[j]->psiInv(uR);
    }
}

void Sealib::SubGraphStack::toptune() {}
