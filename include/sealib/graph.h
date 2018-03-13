//
// Created by jmeintrup on 06.03.18.
//

#include <ostream>
#include "sealib/node.h"

#ifndef SEA_GRAPH_H
#define SEA_GRAPH_H

#endif //SEA_GRAPH_Hs
/**
 * Graph class.
 * A Graph object represents a vector of node objects and it's order.
 * It is just a container to be used by different functions.
 */
class Graph {
private:
    std::vector<Node*> nodes;
    unsigned int order;

public:
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
    Graph(unsigned int** adj_matrix, unsigned int _order);

    /**
     * Getter for a specific node in the nodes vector.
     * @param u index in the nodes vector
     * @return Pointer to the Node in the nodes vector
     */
    Node* getNode(unsigned int u);

    /**
    * Returns the index of the head of u in the nodes vector.
    * @param u index in the nodes vector
    * @param k index in the adjacency vector of node u
    * @return index of the head vertex in the nodes vector
    */
    unsigned int head(unsigned int u, unsigned int k);

    /**
    * Order is the number of nodes in the graph.
    * @return order of the graph as int
    */
    unsigned int getOrder();
};