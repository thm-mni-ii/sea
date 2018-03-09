//
// Created by jmeintrup on 06.03.18.
//

#include <iostream>
#include "sealib/node.h"

/**
 * Constructor for the Node class.
 * Takes the adjacency vector and creates a new TrailStructure d
 * and fills d.u with new MatchedInt objects with values corresponding to adj.
 * @param _adj adjacency vector of this node.
 */
Node::Node(std::vector<int> _adj) {
    d = TrailStructure();
    adj = _adj;
    for(int i: adj) {
        d.add_to_u(new MatchedInt(i));
    }
}

/**
 * getter for the trail structure d
 * @return  TrailStructure pointer to d
 */
TrailStructure *Node::get_d() {
    return &d;
}

/**
 * getter for the adjacency vector of this node
 * @return vector of adjacent nodes
 */
std::vector<int> *Node::getAdj() {
    return &adj;
}
