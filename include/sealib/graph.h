//
// Created by jmeintrup on 06.03.18.
//

#include <ostream>
#include "sealib/node.h"

#ifndef SEA_GRAPH_H
#define SEA_GRAPH_H

#endif //SEA_GRAPH_Hs

class Graph {
private:
    std::vector<Node*> nodes;
    std::vector<int> t;

    //void Graph::extend(Node *e, std::vector<int> *_t);

public:
    Graph();
    Graph(std::vector<std::vector<int>> adj_matrix);
    std::vector<Node*> *get_nodes();
    //int order();
    //int deg(int u);
    int head(int u, int k);

    void hierholzer();

};