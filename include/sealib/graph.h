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

    void hierholzer_extend(Node *e);

public:
    /**
     * Constructor for the graph, takes an adj_matrix as value in the following format:
     * node x node matrix, entry in matrix is number of edges to that node.
     *
     * example:
     *
     *  n        1       2       3
     *      **************************
     *  1   *    0   *   1   *   1   *
     *      **************************
     *  2   *    1   *   0   *   2   *
     *      **************************
     *  3   *    0   *   2   *   1   *
     *      **************************
     */
    Graph::Graph(int** adj_matrix, int order):order(order) {
        nodes = std::vector<Node*>();

        for(int i = 0; i < order; i ++) {
            auto * n = new Node(adj_matrix[i], order);
            nodes.push_back(n);
        }

        for(int i = 0; i < order ; i ++) {
            int deg = nodes.at(i)->get_deg();
            std::vector<Adjacency> *adj_arr = nodes.at(i)->get_adj();
            for(int j = 0; j < deg; j++) {
                if(adj_arr->at(j).index != -1) {
                    int v = adj_arr->at(j).vertex;

                    std::vector<Adjacency> *_adj_arr = nodes.at(v)->get_adj();
                    int _deg = nodes.at(v)->get_deg();
                    for(int _j = 0; _j < _deg;_j++) {
                        if(_adj_arr->at(_j).index == -1 && _adj_arr->at(_j).vertex == i) {
                            _adj_arr->at(_j).index = j;
                            break;
                        }
                    }
                }
            }
        }
    }

    /**
     * Getter for a specific node in the nodes vector.
     * @param u index in the nodes vector
     * @return Pointer to the Node in the nodes vector
     */
    Node* get_node(int u) { return nodes.at(u); }

    /**
    * Returns the index of the head of u in the nodes vector.
    * @param u index in the nodes vector
    * @param k index in the adjacency vector of node u
    * @return index of the head vertex in the nodes vector
    */
    int head(int u, int k) { return nodes.at(u)->get_adj()->at(k).vertex; };

    /**
    * Order is the number of nodes in the graph.
    * @return order of the graph as int
    */
    int get_order() { return order; };

    void hierholzer();
};