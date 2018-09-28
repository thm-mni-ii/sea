#ifndef SEALIB_SUBGRAPHSTACK_H_
#define SEALIB_SUBGRAPHSTACK_H_

#include <sealib/basicgraph.h>
#include <sealib/recursivesubgraph.h>
#include <sealib/basesubgraph.h>
#include <sealib/linkedliststack.h>
#include <memory>
#include <utility>
namespace Sealib {
class SubgraphStack {
 private:
    std::vector<SubGraph *> clientList;
    int fatness;

 public:

    SubgraphStack(std::shared_ptr<BasicGraph> g_) {
        clientList.emplace_back(new BaseSubGraph(g_));
    }
    /**
     * Pushes a new subgraph G_l+1 on G_l.
     * i.e. replaces the client list (G_0,...,G_l) with (G_0,...,G_l,G_l+1)
     * G_l+1 is isomorphic to a proper subgraph of G_l.
     * @param v Bitsequence of length n_l
     * @param e Bitsequence of length 2m_l
     */
    void push(Sealib::Bitset<unsigned char> &v,
              Sealib::Bitset<unsigned char> &e) {
        clientList.emplace_back(v, e);
    };

    /**
     * Replaces the client list (G_0,...,G_l) with (G_0,...,G_l-1).
     * Asserts that the client list is not empty.
     */
    void pop() {
        delete clientList[clientList.size() - 1];
        clientList.pop_back();
    };

    /**
     * Returns the order of G_i
     */
    unsigned int order(unsigned int i) const {
        return clientList[i]->order();
    };

    /**
     * @return - the degree of vertex u in G_i
     */
    unsigned int degree(unsigned int i,
                        unsigned int u) const {
        return clientList[i]->degree(i);
    }

    /**
     * @return - the head of u's k-th outgoing arc in G_i. i.e. head_i(u, k)
     */
    unsigned int head(unsigned int i,
                      unsigned int u,
                      unsigned int k) const {
        return clientList[i]->head(u, k);
    };

    /**
     * @return - the pair that represents the mate of u's kth outgoing arc in G_i, i.e. mate_i(u, k)
     */
    std::tuple<unsigned int, unsigned int> mate(unsigned int i,
                                                unsigned int u,
                                                unsigned int k) const {
        return clientList[i]->mate(u, k);
    };

    virtual ~SubgraphStack() {
        for(SubGraph* g : clientList) {
            delete g;
        }
    }
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPHSTACK_H_
