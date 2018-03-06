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
Node::Node(const int _adj[], int len) {


    d = TrailStructure();

    deg = 0;
    for(int i = 0; i < len; i ++) {
        deg += _adj[i];
    }

    std::cout << "deg: " << deg << std::endl;

    //adj = (CrossLinkedInt**) malloc(sizeof(CrossLinkedInt*) * deg);
    adj = std::vector<CrossLinkedInt>();
    //adj.resize(deg);

    for(int i = 0; i < len; i ++) {
        for(int j = 0; j < _adj[i]; j++ ) {
            adj.push_back(CrossLinkedInt(i));
        }
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


std::vector<CrossLinkedInt> *Node::getAdj() {
    return &adj;
}

int Node::get_deg() {
    return deg;
}
