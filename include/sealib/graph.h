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
    int order;

    //void Graph::extend(Node *e, std::vector<int> *_t);

public:
    Graph(int** adj_matrix, int _order);
    Node* get_node(int u);
    int head(int u, int k);
    int get_order();
    void hierholzer();

};