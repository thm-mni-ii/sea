//
// Created by jmeintrup on 06.03.18.
//

#include "sealib/graph.h"



/**
 * Getter for a specific node in the nodes array.
 * @param u index in the nodes array
 * @return Pointer to the Node in the nodes array
 */
Node *Graph::getNode(size_t u) { return &nodes[u]; }

/**
* Returns the index of the head of u in the nodes vector.
* @param u index in the nodes vector
* @param k index in the adjacency vector of node u
* @return index of the head vertex in the nodes vector
*/
size_t Graph::head(size_t u, size_t k) { return nodes[u].getAdj()[k].vertex; }

/**
* Order is the number of nodes in the graph.
* @return order of the graph as int
*/
size_t Graph::getOrder() { return order; }


/**
 * Constructor for the Graph class.
 * @param _nodes Array of nodes, to be initialized and crosslinked by the caller.
 * @param _order Order of the graph, equals the length of the nodes array.
 */
Graph::Graph(Node *_nodes, size_t _order) : nodes(_nodes), order(_order) {}
