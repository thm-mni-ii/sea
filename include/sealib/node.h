#ifndef SEA_NODE_H
#define SEA_NODE_H

#include <vector>
#include "adjacency.h"

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
};

#endif //SEA_NODE_H
