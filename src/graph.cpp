//
// Created by jmeintrup on 06.03.18.
//

#include "sealib/graph.h"

/**
 * Constructor
 */
Graph::Graph(std::vector<std::vector<int>> adj_matrix) {
    nodes = std::vector<Node*>();
    for(std::vector<int> v : adj_matrix) {
        nodes.push_back(new Node(v));
    }
}

/**
 * Default constructor constructs an empty node vector.
 * Use get_nodes to retrieve a pointer to the vector and then add the nodes of a graph.
 */
Graph::Graph() {
    nodes = std::vector<Node*>();
}

/**
 * getter function for the vector containing all nodes.
 * index in the vector corresponds to the node number.
 * @return pointer to the node vector
 */
std::vector<Node*> *Graph::get_nodes() {
    return &nodes;
}

/**
 * Order is the number of nodes in the graph.
 * Equals the size of the vector containing the nodes.
 * @return order of the graph as int
 */
int Graph::order() {
    return nodes.size();
}

/**
 * Grabs the degree of the node u, u is the index of the node in the nodes vector.
 * Degree equals the size of the adjacency vector of u.
 * @param u index in the nodes vector
 * @return degree of u
 */
int Graph::deg(int u) {
    return nodes.at(u)->getAdj()->size();
}

/**
 * Returns the index of the head of u in the nodes vector.
 * @param u index in the nodes vector
 * @param k index in the adjacency vector of node u
 * @return
 */
int Graph::head(int u, int k) {
    return nodes.at(u)->getAdj()->at(k);;
}

void Graph::hierholzer() {
    /**
     *  while(has non-black)
     *      vertex u = odd OR gray OR white.
     *      if(gray AND even)
     *          a_old = marked arc from u;
     *
     *      e = unmaked edge out of u; mark a_e out of u;
     *      std::vector _t = std::vector<int>();
     *      _t.push(a_e)
     *  
     *      extend(e);
     *
     *      if(a_old) insert T_old into T;
     *
     *
     */
}

void Graph::extend(Node *e, std::vector<int> *_t) {

}


