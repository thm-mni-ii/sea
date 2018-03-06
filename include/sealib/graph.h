//
// Created by jmeintrup on 06.03.18.
//

#include <ostream>
#include "node.h"

#ifndef SEA_GRAPH_H
#define SEA_GRAPH_H

#endif //SEA_GRAPH_Hs

class Graph {
private:
    std::vector<Node*> nodes;

public:
    Graph(std::vector<std::vector<int>> adj_matrix);
    std::vector<Node*> get_nodes();
};