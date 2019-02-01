#ifndef SEALIB_SUBGRAPHSTACK_H_
#define SEALIB_SUBGRAPHSTACK_H_

#include <sealib/graph/undirectedgraph.h>
#include <sealib/collection/bitset.h>
#include <sealib/dictionary/rankselect.h>
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
    friend class SubGraph;
    friend class RecursiveSubGraph;
    friend class BaseSubGraph;
 private:
    static constexpr uint8_t rSize = 7;
    static std::vector<uint64_t> refs;
    std::vector<SubGraph *> clientList;
    uint64_t currentRef;
    uint64_t tuned;

    RankSelect *tunedPhi0;
    RankSelect *tunedPsi0;
    RankSelect *tunedPhi;
    RankSelect *tunedPsi;

    void tunephi0(uint64_t i);
    void tunepsi0(uint64_t i);
    void tunephi(uint64_t i);
    void tunepsi(uint64_t i);

 public:
    explicit SubGraphStack(std::shared_ptr<UndirectedGraph> g_);
    /**
     * Pushes a new subgraph G_l+1 on G_l.
     * i.e. replaces the client list (G_0,...,G_l) with (G_0,...,G_l,G_l+1)
     * G_l+1 is isomorphic to a proper subgraph of G_l.
     * @param v Bitsequence of length n_l
     * @param a Bitsequence of length 2m_l
     */
    void push(const Sealib::Bitset<uint8_t> &v,
              const Sealib::Bitset<uint8_t> &a);

    /**
     * Pushes a new subgraph G_l+1 on G_l.
     * i.e. replaces the client list (G_0,...,G_l) with (G_0,...,G_l,G_l+1)
     * G_l+1 is isomorphic to a proper subgraph of G_l.
     * The bitset for the vertices will be created from the arc bitset.
     * With this version of push there are no vertices without arcs.
     * Each vertice that still has an outgoing arc will be set in the bitset v.
     * @param a Bitsequence of length 2m_l
     */
    void push(const Sealib::Bitset<uint8_t> &a);

    /**
     * Replaces the client list (G_0,...,G_l) with (G_0,...,G_l-1).
     * Asserts that the client list is not empty.
     */
    void pop();

    /**
     * Returns the order of G_i
     */
    uint64_t order(uint64_t i) const;

    inline uint64_t order() const {
        return order(clientList.size() - 1);
    }

    /**
     * @return - the degree of vertex u in G_i
     */
    uint64_t degree(uint64_t i,
                         uint64_t u) const;

    /**
     * @return - the degree of vertex u for G_l = the top graph on the stack
     */
    inline uint64_t degree(uint64_t u) const {
        return degree(clientList.size() - 1, u);
    }

    /**
     * @return - the head of u's k-th outgoing arc in G_i. i.e. head_i(u, k)
     */
    uint64_t head(uint64_t i,
                       uint64_t u,
                       uint64_t k) const;

    /**
     * @return - the head of u's k-th outgoing arc in G_l = the top graph on the stack
     */
    inline uint64_t head(uint64_t u,
                              uint64_t k) const {
        return head(clientList.size() - 1, u, k);
    }

    /**
     * @return - the pair that represents the mate of u's kth outgoing arc in G_i, i.e. mate_i(u, k)
     */
    std::tuple<uint64_t, uint64_t> mate(uint64_t i,
                                                  uint64_t u,
                                                  uint64_t k) const;

    /**
     * @return - the pair that represents the mate of u's kth outgoing arc in G_l = the top graph on the stack
     */
    inline std::tuple<uint64_t, uint64_t> mate(uint64_t u,
                                                         uint64_t k) const {
        return mate(clientList.size() - 1, u, k);
    }

    /**
     * @return the arc number of the k'th outgoing arc in vertex u in G_i.
     */
    uint64_t g(uint64_t i, uint64_t u, uint64_t k) const;

    inline uint64_t g(uint64_t u, uint64_t k) const {
        return g(clientList.size() - 1, u, k);
    }

    /**
     * @return the number of arcs the subgraph G_i has
     */
    uint64_t gMax(uint64_t i) const;

    inline uint64_t gMax() const {
        return gMax(clientList.size() - 1);
    }

    /**
     * @return the (node, arc) pair beint32_ting to the r'th arc in G_i
     */
    std::tuple<uint64_t, uint64_t> gInv(uint64_t i, uint64_t r) const;

    /**
     * @return the (node, arc) pair beint32_ting to the r'th arc in G_l = the top graph on the stack
     */
    inline std::tuple<uint64_t, uint64_t> gInv(uint64_t r) const {
        return gInv(clientList.size() - 1, r);
    }

    /**
     * @return translation of the u'th node in G_i to the isomorph node in G_j
     */
    uint64_t phi(uint64_t i, uint64_t j, uint64_t u) const;

    /**
     * @return translation of the a'th arc in G_i to the isomorph arc in G_j
     */
    uint64_t psi(uint64_t i, uint64_t j, uint64_t a)const;

    /**
     * @return translation of the u'th node in G_l = the top graph to the isomorph node in G_j
     */
    inline uint64_t phi(uint64_t j, uint64_t u) const {
        return phi(clientList.size() - 1, j, u);
    }

    /**
     * @return translation of the a'th arc in G_l = the top graph of to the isomorph arc in G_j
     */
    inline uint64_t psi(uint64_t j, uint64_t a) const {
        return psi(clientList.size() - 1, j, a);
    }

    /**
     * @return translation of the u'th node in G_l = the top graph to the isomorph node in G_0
     */
    inline uint64_t phi(uint64_t u) const {
        return phi(clientList.size() - 1, 0, u);
    }

    /**
     * @return translation of the a'th arc in G_l = the top graph of to the isomorph arc in G_0
     */
    inline uint64_t psi(uint64_t a) const {
        return psi(clientList.size() - 1, 0, a);
    }

    /**
     * @return  translation of the u'th node in G_0 to G_l = the top graph
     */
    inline uint64_t phiInv(uint64_t u) const {
        return phi(0, clientList.size() - 1, u);
    }

    /**
     * @return translation of the a'th arc in G_0 to G_l = the top graph
     */
    inline uint64_t psiInv(uint64_t a) const {
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
    void tune(uint64_t i);

    /**
     * @return the number of graphs currently on the stack
     */
    inline uint64_t size() const {
        return clientList.size();
    }

    virtual ~SubGraphStack();
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPHSTACK_H_
