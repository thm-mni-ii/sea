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
    Adjacency* adj;
    size_t deg;


public:

    /**
     * Constructor for the Node class.
     * @param _adj array of adjacencys of this node.
     * @param _deg degree of this node.
    */
    Node(Adjacency *_adj, size_t _deg);

    /**
     * Getter for the adjacency array. Returns a pointer to the first element of the array.
     * @return pointer to adj
     */
    Adjacency *getAdj();

    /**
     * Getter for the degree of the node.
     * @return value of deg
     */
    size_t getDeg();
};
#endif //SEA_NODE_H
