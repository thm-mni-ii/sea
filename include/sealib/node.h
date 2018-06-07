#ifndef SEA_NODE_H
#define SEA_NODE_H

#include <vector>
#include "sealib/adjacency.h"

/**
 * A Node object containing the nodes adjacencies and its degree.
 * @author Johannes Meintrup
 */
namespace Sealib {
class Node {
 private:
    std::vector<Sealib::Adjacency> adj;

 public:
    /**
     * Creates a node out of its adjacencies.
     * @param _adj array of adjacencies.
     * @param _deg degree of this node.
    */
    Node();

    /**
     * @return Returns a ref to the adjacency vector.
     */
    std::vector<Sealib::Adjacency> &getAdj();

    /**
     * @param i idx of the adjacency
     * @return Returns a ref to the adjacency in the vector at i.
     */
    const Sealib::Adjacency& getAdj(unsigned int i) const;

    /**
     * @return Returns the degree of the node.
     */
    unsigned int getDegree() const;

    void setAdjacencies(std::vector<Adjacency> adj_);
};
}  // namespace Sealib

#endif  // SEA_NODE_H
