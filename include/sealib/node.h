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
    TrailStructure d;
    int deg;
    std::vector<CrossLinkedInt> adj;

public:
    std::vector<CrossLinkedInt>* getAdj();
    TrailStructure *get_d();
    int get_deg();
    Node(const int *_adj, int len);
};


#endif //SEA_NODE_H
