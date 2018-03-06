//
// Created by jmeintrup on 06.03.18.
//

#ifndef SEA_NODE_H
#define SEA_NODE_H


#include <vector>
#include <ostream>
#include "trailstructure.h"

class Node {

private:
    std::vector<int> adj;
    TrailStructure d;
public:
    std::vector<int> getAdj();
    TrailStructure *get_d();

    Node(std::vector<int> adj);
};


#endif //SEA_NODE_H
