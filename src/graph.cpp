//
// Created by jmeintrup on 06.03.18.
//

#include <limits>
#include "sealib/graph.h"



/**
 * Getter for a specific node in the nodes array.
 * @param u index in the nodes array
 * @return Pointer to the Node in the nodes array
 */
Node *Graph::getNode(unsigned int u) { return &nodes[u]; }

/**
* Returns the index of the head of u in the nodes vector.
* @param u index in the nodes vector
* @param k index in the adjacency vector of node u
* @return index of the head vertex in the nodes vector
*/
unsigned int Graph::head(unsigned int u, unsigned int k) { return nodes[u].getAdj()[k].vertex; }

/**
* Order is the number of nodes in the graph.
* @return order of the graph as int
*/
unsigned int Graph::getOrder() { return order; }


/**
 * Constructor for the Graph class.
 * Initiliazes the crossIndexes of the adjacencies of the nodes.
 * @param _nodes Array of nodes, to be initialized by the caller.
 * @param _order Order of the graph, equals the length of the nodes array.
 */
Graph::Graph(Node *_nodes, unsigned int _order) : nodes(_nodes), order(_order) {
    for(unsigned int i = 0; i < order ; i ++) {

        const unsigned int deg = nodes[i].getDeg();
        Adjacency *adj_arr = nodes[i].getAdj();

        for(unsigned int j = 0; j < deg; j++) {
            if(adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {

                unsigned int v = adj_arr[j].vertex;
                Adjacency *_adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDeg();

                for(unsigned int _j = 0; _j < _deg;_j++) {
                    if(_adj_arr[_j].crossIndex == std::numeric_limits<unsigned int>::max() && _adj_arr[_j].vertex == i) {
                        _adj_arr[_j].crossIndex = j;
                        adj_arr[j].crossIndex = _j;
                        break;
                    }
                }
            }
        }
    }
}
