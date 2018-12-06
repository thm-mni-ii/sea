#ifndef SEALIB_GRAPH_NODE_H_
#define SEALIB_GRAPH_NODE_H_

#include <sealib/graph/adjacency.h>
#include <cstdint>
#include <vector>

namespace Sealib {
/**
 * A Node object containing the nodes adjacencies and its degree.
 * @author Johannes Meintrup
 */
template <class T>
class Node {
 public:
    Node() : adj(0) {}
    explicit Node(std::vector<T> const &p) : adj(p) {}
    uint getDegree() const { return adj.size(); }
    std::vector<T> const &getAdj() const { return adj; }
    std::vector<T> &getAdj() { return adj; }
    void addAdjacency(T const &v) { adj.emplace_back(v); }
 private:
    std::vector<T> adj;
};

typedef Node<uint>
    NodeD;
typedef Node<std::pair<uint, uint>> NodeU;

// class UndirectedNode : Node<std::pair<uint,uint>> {
// public:
//    /**
//     * Creates a node out of its adjacencies.
//     * @param _adj vector of adjacencies.
//    */
//    explicit Node(const std::vector<Sealib::Adjacency> &adj_);
//
//    /**
//     * @return Returns a pointer to the adjacency array.
//     */
//    const std::vector<Adjacency> &getAdj() const;
//
//    /**
//     * @return Returns the degree of the node.
//     */
//    uint32_t getDegree() const;
//
//    /**
//     * Make a cross pointer from "this" to an "other" adjacency array
//     * @param adjIndex index of endpoint in "this" adjacency array
//     * @param crossIndex index of endpoint in the "other" adjacency array
//     */
//     void setCrossIndex(uint32_t adjIndex, uint32_t crossIndex);
//
//     /**
//      * Adds a new Adjacency.
//      * @param vertex - vertex of the Adjacency.
//      * Set the crossindex manually after adding a new adjacency
//      */
//      void addAdjacency(uint32_t vertex);
//};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_NODE_H_
