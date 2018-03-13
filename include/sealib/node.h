//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_NODE_H
#define SEA_NODE_H


#include <vector>
#include <ostream>
#include "adjacency.h"

class Node {

private:
    int deg;
    std::vector<Adjacency> adj;

public:
    /**
     * Constructor for the Node class.
     * Takes the adjacency vector and creates a new TrailStructure d
     * and fills d.u with new MatchedInt objects with values corresponding to adj.
     * @param _adj adjacency vector of this node.
    */
    Node::Node(const int _adj[], int len) {
        deg = 0;
        for(int i = 0; i < len; i ++) {
            deg += _adj[i];
        }
        adj = std::vector<Adjacency>();

        for(int i = 0; i < len; i ++) {
            for(int j = 0; j < _adj[i]; j++ ) {
                adj.push_back(Adjacency(i));
            }
        }
    }

    /**
     * Getter for the adjacency vector. Returns a pointer to the vector
     * @return pointer to adj
     */
    std::vector<Adjacency> *get_adj() { return &adj; }

    /**
     * Getter for the degree of the node.
     * @return value of deg
     */
    int get_deg() { return deg; };
};
#endif //SEA_NODE_H
