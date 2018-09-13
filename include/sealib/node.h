#ifndef SEALIB_NODE_H_
#define SEALIB_NODE_H_

#include <vector>
#include <sealib/adjacency.h>

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
    Node(const Adjacency *adj_, unsigned long deg_);

    /**
     * Creates a node out of its adjacencies.
     * @param _adj vector of adjacencies.
    */
    Node(const std::vector<Sealib::Adjacency> &adj_);

    /**
     * @return Returns a pointer to the adjacency array.
     */
    const std::vector<Adjacency> &getAdj();

    /**
     * @return Returns the degree of the node.
     */
    unsigned int getDegree();

    /**
     * Sets the crossindex in the adjacency
     */
     void setCrossIndex(unsigned int adjIndex, unsigned int crossIndex);
};
}  // namespace Sealib
#endif  // SEALIB_NODE_H_
