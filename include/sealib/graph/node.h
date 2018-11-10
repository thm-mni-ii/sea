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
class Node {
 private:
    std::vector<Sealib::Adjacency> adj;

 public:
    /**
     * Creates a node out of its adjacencies.
     * @param _adj array of adjacencies.
     * @param _deg degree of this node.
    */
    Node(const Adjacency *adj_, uint64_t deg_);

    /**
     * Creates a node out of its adjacencies.
     * @param _adj vector of adjacencies.
    */
    explicit Node(const std::vector<Sealib::Adjacency> &adj_);

    Node() = default;

    /**
     * @return Returns a pointer to the adjacency array.
     */
    const std::vector<Adjacency> &getAdj() const;

    /**
     * @return Returns the degree of the node.
     */
    uint32_t getDegree() const;

    /**
     * Make a cross pointer from "this" to an "other" adjacency array
     * @param adjIndex index of endpoint in "this" adjacency array
     * @param crossIndex index of endpoint in the "other" adjacency array
     */
     void setCrossIndex(uint32_t adjIndex, uint32_t crossIndex);

     /**
      * Adds a new Adjacency.
      * @param vertex - vertex of the Adjacency.
      * Set the crossindex manually after adding a new adjacency
      */
      void addAdjacency(uint32_t vertex);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_NODE_H_
