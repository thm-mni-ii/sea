//
// Created by jmeintrup on 13.03.18.
//
#include "sealib/node.h"

/**
 * Getter for the adjacency vector. Returns a pointer to the vector
 * @return pointer to adj
 */
std::vector<Adjacency> *Node::get_adj() { return &adj; }

/**
 * Getter for the degree of the node.
 * @return value of deg
 */
unsigned int Node::get_deg() { return deg; }

/**
     * Constructor for the Node class.
     * Takes the adjacency vector and creates a new TrailStructure d
     * and fills d.u with new MatchedInt objects with values corresponding to adj.
     * @param _adj adjacency vector of this node.
    */
Node::Node(const unsigned int *_adj, unsigned int len) {
    deg = 0;
    for(unsigned int i = 0; i < len; i ++) {
        deg += _adj[i];
    }
    adj = std::vector<Adjacency>();

    for(unsigned int i = 0; i < len; i ++) {
        for(unsigned int j = 0; j < _adj[i]; j++ ) {
            adj.push_back(Adjacency(i));
        }
    }
}
