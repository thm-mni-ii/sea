#ifndef SEALIB_SUBGRAPHSTACK_H_
#define SEALIB_SUBGRAPHSTACK_H_

#include <sealib/basicgraph.h>
#include <sealib/bitset.h>
#include <sealib/rankselect.h>
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
/**
 * A space efficient implementation of a subgraph stack.
 * A SubGraphStack object maintains a list of Graphs (G_0,...,G_l),
 * such that for 0 < i <= l G_i is a subgraph of G_i-1.
 * All operations can be evaluated in effectively constant time, the subgraphs that are being pushed
 * are represented by various bitsets with at most O(|V_0| + 2|E_0|) size
 * for which we construct additional rank-select structures.
 */
class SubGraphStack {
    static unsigned long refs[6];

    friend class SubGraph;
    friend class RecursiveSubGraph;
    friend class BaseSubGraph;
 private:
    std::vector<SubGraph *> clientList;
    unsigned long currentRef;
    unsigned long tuned;

    RankSelect<> *tunedPhi0;
    RankSelect<> *tunedPsi0;
    RankSelect<> *tunedPhi;
    RankSelect<> *tunedPsi;

    void tunephi0(unsigned long i);
    void tunepsi0(unsigned long i);
    void tunephi(unsigned long i);
    void tunepsi(unsigned long i);

 public:
    explicit SubGraphStack(std::shared_ptr<BasicGraph> g_);
    /**
     * Pushes a new subgraph G_l+1 on G_l.
     * i.e. replaces the client list (G_0,...,G_l) with (G_0,...,G_l,G_l+1)
     * G_l+1 is isomorphic to a proper subgraph of G_l.
     * @param v Bitsequence of length n_l
     * @param a Bitsequence of length 2m_l
     */
    void push(const Sealib::Bitset<unsigned char> &v,
              const Sealib::Bitset<unsigned char> &a);

    /**
     * Pushes a new subgraph G_l+1 on G_l.
     * i.e. replaces the client list (G_0,...,G_l) with (G_0,...,G_l,G_l+1)
     * G_l+1 is isomorphic to a proper subgraph of G_l.
     * The bitset for the vertices will be created from the arc bitset.
     * With this version of push there are no vertices without arcs.
     * Each vertice that still has an outgoing arc will be set in the bitset v.
     * @param a Bitsequence of length 2m_l
     */
    void push(const Sealib::Bitset<unsigned char> &a);

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
    }

    /**
     * @return the arc number of the k'th outgoing arc in vertex u in G_i.
     */
    unsigned long g(unsigned long i, unsigned long u, unsigned long k) const;

    inline unsigned long g(unsigned long u, unsigned long k) const {
        return g(clientList.size() - 1, u, k);
    }

    /**
     * @return the number of arcs the subgraph G_i has
     */
    unsigned long gMax(unsigned long i) const;

    inline unsigned long gMax() const {
        return gMax(clientList.size() - 1);
    }

    /**
     * @return the (node, arc) pair belonging to the r'th arc in G_i
     */
    std::tuple<unsigned long, unsigned long> gInv(unsigned long i, unsigned long r) const;

    /**
     * @return the (node, arc) pair belonging to the r'th arc in G_l = the top graph on the stack
     */
    inline std::tuple<unsigned long, unsigned long> gInv(unsigned long r) const {
        return gInv(clientList.size() - 1, r);
    }

    /**
     * @return translation of the u'th node in G_i to the isomorph node in G_j
     */
    unsigned long phi(unsigned long i, unsigned long j, unsigned long u) const;

    /**
     * @return translation of the a'th arc in G_i to the isomorph arc in G_j
     */
    unsigned long psi(unsigned long i, unsigned long j, unsigned long a)const;

    /**
     * @return translation of the u'th node in G_l = the top graph to the isomorph node in G_j
     */
    inline unsigned long phi(unsigned long j, unsigned long u) const {
        return phi(clientList.size() - 1, j, u);
    }

    /**
     * @return translation of the a'th arc in G_l = the top graph of to the isomorph arc in G_j
     */
    inline unsigned long psi(unsigned long j, unsigned long a) const {
        return psi(clientList.size() - 1, j, a);
    }

    /**
     * @return translation of the u'th node in G_l = the top graph to the isomorph node in G_0
     */
    inline unsigned long phi(unsigned long u) const {
        return phi(clientList.size() - 1, 0, u);
    }

    /**
     * @return translation of the a'th arc in G_l = the top graph of to the isomorph arc in G_0
     */
    inline unsigned long psi(unsigned long a) const {
        return psi(clientList.size() - 1, 0, a);
    }

    /**
     * @return  translation of the u'th node in G_0 to G_l = the top graph
     */
    inline unsigned long phiInv(unsigned long u) const {
        return phi(0, clientList.size() - 1, u);
    }

    /**
     * @return translation of the a'th arc in G_0 to G_l = the top graph
     */
    inline unsigned long psiInv(unsigned long a) const {
        return psi(0, clientList.size() - 1, a);
    }

    /**
     * Speeds up the calls of phi, psi, g, gInv, head, mate, order and degree
     * for the graph G_l that is currently on the top of the stack.
     * This is done by creating rankSelect structures for the direct translation between G_0 and G_l and G_l and G_l - 1
     */
    inline void toptune() {
        tune(clientList.size()-1);
    }

    /**
     * Speeds up the calls of phi, psi, g, gInv, head, mate, order and degree
     * for the graph G_i
     * This is done by creating rankSelect structures for the direct translation between G_0 and G_i, and G_i and G_i - 1
     *
     * If there are previously initialized tuning structures, they are destroyed.
     * @param i - idx of the Graph to be tuned
     */
    void tune(unsigned long i);

    /**
     * @return the number of graphs currently on the stack
     */
    inline unsigned long size() const {
        return clientList.size();
    }

    virtual ~SubGraphStack();
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPHSTACK_H_
