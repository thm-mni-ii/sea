//
// Created by jmeintrup on 06.03.18.
//

#include <limits>
#include "sealib/graph.h"



/**
 * Constructor for the graph, takes an adj_matrix as value in the following format:
 * node x node matrix, entry in matrix is number of edges to that node.
 *
 * example:
 *
 * n         1       2       3
 *      **************************
 *  1   *    0   *   1   *   1   *
 *      **************************
 *  2   *    1   *   0   *   2   *
 *      **************************
 *  3   *    0   *   2   *   1   *
 *      **************************
 * @param adj_matrix NxN adjcacency matrix representation of the graph.
 * @param _order Order of the graph, order equals the number of nodes.
 */
Graph::Graph(unsigned int** adj_matrix, unsigned int _order):order(_order) {
    nodes = std::vector<Node*>();

    for(unsigned int i = 0; i < order; i ++) {
        auto * n = new Node(adj_matrix[i], order);
        nodes.push_back(n);
    }

    for(unsigned int i = 0; i < order ; i ++) {
        unsigned int deg = nodes.at(i)->getDeg();
        std::vector<Adjacency> *adj_arr = nodes.at(i)->getAdj();

        for(unsigned int j = 0; j < deg; j++) {
            if(adj_arr->at(j).crossIndex == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr->at(j).vertex;

                std::vector<Adjacency> *_adj_arr = nodes.at(v)->getAdj();
                unsigned int _deg = nodes.at(v)->getDeg();
                for(unsigned int _j = 0; _j < _deg;_j++) {
                    if(_adj_arr->at(_j).crossIndex == std::numeric_limits<unsigned int>::max() && _adj_arr->at(_j).vertex == i) {
                        _adj_arr->at(_j).crossIndex = j;
                        adj_arr->at(j).crossIndex = _j;
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
Node *Graph::getNode(unsigned int u) { return nodes.at(u); }

/**
    * Returns the index of the head of u in the nodes vector.
    * @param u index in the nodes vector
    * @param k index in the adjacency vector of node u
    * @return index of the head vertex in the nodes vector
    */
unsigned int Graph::head(unsigned int u, unsigned int k) { return nodes.at(u)->getAdj()->at(k).vertex; }

/**
    * Order is the number of nodes in the graph.
    * @return order of the graph as int
    */
unsigned int Graph::getOrder() { return order; }
