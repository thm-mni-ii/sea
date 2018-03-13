//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_NODE_H
#define SEA_NODE_H


#include <vector>
#include "adjacency.h"
/**
 * Node class.
 * A Node object represents a node's edges and it's degree.
 * It is used by the graph class.
 */
class Node {

private:
    unsigned int deg;
    std::vector<Adjacency> adj;

public:

    /**
     * Constructor for the Node class.
     * Takes the adjacency vector and creates a new TrailStructure d
     * and fills d.u with new MatchedInt objects with values corresponding to adj.
     * @param _adj adjacency vector of this node.
    */
    Node(const unsigned int _adj[], unsigned int len);

    /**
     * Getter for the adjacency vector. Returns a pointer to the vector
     * @return pointer to adj
     */
    std::vector<Adjacency> *getAdj();

    /**
     * Getter for the degree of the node.
     * @return value of deg
     */
    unsigned int getDeg();
};
#endif //SEA_NODE_H
