//
// Created by jmeintrup on 06.03.18.
//

#include <sealib/trailstructure.h>
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
 */
Graph::Graph(unsigned int** adj_matrix, unsigned int _order):order(_order) {
    nodes = std::vector<Node*>();

    for(unsigned int i = 0; i < order; i ++) {
        auto * n = new Node(adj_matrix[i], order);
        nodes.push_back(n);
    }

    for(unsigned int i = 0; i < order ; i ++) {
        unsigned int deg = nodes.at(i)->get_deg();
        std::vector<Adjacency> *adj_arr = nodes.at(i)->get_adj();

        for(unsigned int j = 0; j < deg; j++) {
            if(adj_arr->at(j).index == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr->at(j).vertex;

                std::vector<Adjacency> *_adj_arr = nodes.at(v)->get_adj();
                unsigned int _deg = nodes.at(v)->get_deg();
                for(unsigned int _j = 0; _j < _deg;_j++) {
                    if(_adj_arr->at(_j).index == std::numeric_limits<unsigned int>::max() && _adj_arr->at(_j).vertex == i) {
                        _adj_arr->at(_j).index = j;
                        adj_arr->at(j).index = _j;
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
Node *Graph::get_node(unsigned int u) { return nodes.at(u); }

/**
    * Returns the index of the head of u in the nodes vector.
    * @param u index in the nodes vector
    * @param k index in the adjacency vector of node u
    * @return index of the head vertex in the nodes vector
    */
unsigned int Graph::head(unsigned int u, unsigned int k) { return nodes.at(u)->get_adj()->at(k).vertex; }

/**
    * Order is the number of nodes in the graph.
    * @return order of the graph as int
    */
unsigned int Graph::get_order() { return order; }

/**
 * TODO: Implement hierholzer algorithm
 */
void Graph::hierholzer() {
    /**
     *  while(has non-black)
     *      vertex u = odd OR gray OR white.
     *      if(gray AND even)
     *          a_old = marked arc from u;
     *
     *      e = unmaked edge out of u; mark k_a_e out of u;
     *      std::vector _t = std::vector<int>();
     *      _t.push(a_e)
     *
     *      extend(e);
     *
     *      if(a_old) insert T_old into T;
     */
    /*TrailStructure* trailStructures[order];
    for(unsigned int i = 0; i < order; i++) {
        trailStructures[i] = new TrailStructure();
        for(unsigned int j = 0; j < nodes.at(i)->get_deg(); j++) {
            trailStructures[i]->add_to_u(new CrossLinkedInt(i));
        }
    }*/
}

/**
 * TODO: Helper function for hierholzer
 * @param e
 */
/*void Graph::hierholzer_extend(Node *e) {

}*/
