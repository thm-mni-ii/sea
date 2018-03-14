//
// Created by jmeintrup on 06.03.18.
//



#ifndef SEA_GRAPH_H
#define SEA_GRAPH_H

#include "sealib/node.h"

/**
 * Graph class.
 * A Graph object represents a array of node objects and it's order.
 * It's a container to be used by different functions.
 */
class Graph {
private:
    Node* nodes;
    unsigned int order;

public:
    /**
     * Constructor for the Graph class.
     * @param _nodes Array of nodes, to be initialized by the caller.
     * @param _order Order of the graph, equals the length of the nodes array.
     */
    Graph(Node* _nodes, unsigned int _order);

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array
     * @return Pointer to the Node in the nodes array
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
#endif //SEA_GRAPH_Hs