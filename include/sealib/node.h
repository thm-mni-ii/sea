#ifndef SEALIB_NODE_H_
#define SEALIB_NODE_H_

#include <vector>
#include "sealib/adjacency.h"

namespace Sealib {
/**
 * A Node object containing the nodes adjacencies and its degree.
 * @author Johannes Meintrup
 */
class Node {
 private:
    Adjacency *adj;
    unsigned int deg;

 public:
    /**
     * Creates a node out of its adjacencies.
     * @param _adj array of adjacencies.
     * @param _deg degree of this node.
    */
    Node(Adjacency *_adj, unsigned int _deg);

    /**
     * @return Returns a pointer to the adjacency array.
     */
    Adjacency *getAdj();

    /**
     * @return Returns the degree of the node.
     */
    unsigned int getDegree();

    /**
     * The name of this node's parent node.
     * added by: Simon Heuser
     */
    unsigned int parent = 0;
};
}  // namespace Sealib
#endif  // SEALIB_NODE_H_
