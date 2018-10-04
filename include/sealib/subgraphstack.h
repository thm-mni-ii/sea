#ifndef SEALIB_SUBGRAPHSTACK_H_
#define SEALIB_SUBGRAPHSTACK_H_

#include <sealib/basicgraph.h>
#include <sealib/bitset.h>
#include <memory>
#include <utility>
#include <vector>
#include <tuple>

namespace Sealib {
class SubGraph;
class BaseSubGraph;
class RecursiveSubGraph;
}

namespace Sealib {
class SubGraphStack {
    friend class SubGraph;
    friend class RecursiveSubGraph;
    friend class BaseSubGraph;

 private:
    std::vector<SubGraph *> clientList;

    unsigned long currentRef;
    static unsigned long refs[6] = {0, 1, 3, 15, 65535, (unsigned long) - 1};

 public:
    explicit SubGraphStack(std::shared_ptr<BasicGraph> g_);
    /**
     * Pushes a new subgraph G_l+1 on G_l.
     * i.e. replaces the client list (G_0,...,G_l) with (G_0,...,G_l,G_l+1)
     * G_l+1 is isomorphic to a proper subgraph of G_l.
     * @param v Bitsequence of length n_l
     * @param e Bitsequence of length 2m_l
     */
    void push(const Sealib::Bitset<unsigned char> &v,
              const Sealib::Bitset<unsigned char> &e);

    /**
     * Replaces the client list (G_0,...,G_l) with (G_0,...,G_l-1).
     * Asserts that the client list is not empty.
     */
    void pop();

    /**
     * Returns the order of G_i
     */
    unsigned long order(unsigned long i) const;

    inline unsigned long order() const {
        return order(clientList.size() - 1);
    }

    /**
     * @return - the degree of vertex u in G_i
     */
    unsigned long degree(unsigned long i,
                         unsigned long u) const;

    /**
     * @return - the degree of vertex u for G_l = the top graph on the stack
     */
    inline unsigned long degree(unsigned long u) const {
        return degree(clientList.size() - 1, u);
    }

    /**
     * @return - the head of u's k-th outgoing arc in G_i. i.e. head_i(u, k)
     */
    unsigned long head(unsigned long i,
                       unsigned long u,
                       unsigned long k) const;

    /**
     * @return - the head of u's k-th outgoing arc in G_l = the top graph on the stack
     */
    inline unsigned long head(unsigned long u,
                              unsigned long k) const {
        return head(clientList.size() - 1, u, k);
    }

    /**
     * @return - the pair that represents the mate of u's kth outgoing arc in G_i, i.e. mate_i(u, k)
     */
    std::tuple<unsigned long, unsigned long> mate(unsigned long i,
                                                  unsigned long u,
                                                  unsigned long k) const;

    /**
     * @return - the pair that represents the mate of u's kth outgoing arc in G_l = the top graph on the stack
     */
    inline std::tuple<unsigned long, unsigned long> mate(unsigned long u,
                                                         unsigned long k) const {
        return mate(clientList.size() - 1, u, k);
    };

    /**
     * @return the arc number of the k'th outgoing arc in vertex u in G_i.
     */
    unsigned long g(unsigned long i, unsigned long u, unsigned long k);

    inline unsigned long g(unsigned long u, unsigned long k) {
        return g(clientList.size() - 1, u, k);
    };


    /**
     * @return the (node, arc) pair belonging to the r'th arc in G_i
     */
    std::tuple<unsigned long, unsigned long> gInv(unsigned long i, unsigned long r);

    /**
     * @return the (node, arc) pair belonging to the r'th arc in G_l = the top graph on the stack
     */
    inline std::tuple<unsigned long, unsigned long> gInv(unsigned long r) {
        return gInv(clientList.size() - 1, r);
    };

    /**
     * @return translation of the u'th node in G_i to the isomorph node in G_j
     */
    unsigned long phi(unsigned long i, unsigned long j, unsigned long u);

    /**
     * @return translation of the a'th arc in G_i to the isomorph arc in G_j
     */
    unsigned long psi(unsigned long i, unsigned long j, unsigned long a);

    /**
     * Speeds up the calls of phi, psi, g, gInv, head, mate, order and degree
     * for the graph G_l that is currently on the top of the stack.
     * This is done by creating rankSelect structures for the direct translation between G_0 and G_l
     */
    void toptune();

    virtual ~SubGraphStack();
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPHSTACK_H_
