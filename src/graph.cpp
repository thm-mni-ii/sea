//
// Created by jmeintrup on 06.03.18.
//

#include "sealib/graph.h"

Graph::Graph(std::vector<std::vector<int>> adj_matrix) {
    nodes = std::vector<Node*>();
    for(std::vector<int> v : adj_matrix) {
        nodes.push_back(new Node(v));
    }

}

std::vector<Node*> Graph::get_nodes() {
    return nodes;
}


