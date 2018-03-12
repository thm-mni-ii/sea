//
// Created by jmeintrup on 06.03.18.
//

#include <iostream>
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
Graph::Graph(int** adj_matrix, int _order) {

    order = _order;
    nodes = std::vector<Node*>();

    for(int i = 0; i < order; i ++) {
        auto * n = new Node(adj_matrix[i], order);
        nodes.push_back(n);
    }

    for(int i = 0; i < order ; i ++) {
        int deg = nodes.at(i)->get_deg();
        std::vector<CrossLinkedInt> *adj_arr = nodes.at(i)->getAdj();
        for(int j = 0; j < deg; j++) {
            if(!adj_arr->at(j).has_match()) {
                int v = adj_arr->at(j).get_value();

                std::vector<CrossLinkedInt> *_adj_arr = nodes.at(v)->getAdj();
                int _deg = nodes.at(v)->get_deg();
                for(int _j = 0; _j < _deg;_j++) {
                    if(!_adj_arr->at(_j).has_match() && _adj_arr->at(_j).get_value() == i) {
                        _adj_arr->at(_j).match(&adj_arr->at(j));
                        break;
                    }
                }
            }
        }
    }
}




/**
 * Order is the number of nodes in the graph.
 * @return order of the graph as int
 */
int Graph::get_order() {
    return order;
}

/**
 * Grabs the degree of the node u, u is the index of the node in the nodes vector.
 * Degree equals the size of the adjacency vector of u.
 * @param u index in the nodes vector
 * @return degree of u
 */
//int Graph::deg(int u) {
//    return nodes.at(u)->getAdj()->size();
//}

/**
 * Returns the index of the head of u in the nodes vector.
 * @param u index in the nodes vector
 * @param k index in the adjacency vector of node u
 * @return
 */
int Graph::head(int u, int k) {
    return nodes.at(u)->getAdj()->at(k).get_value();
}

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
    while(true) {
        Node *u = nullptr;
        CrossLinkedInt* k_old = nullptr;
        for(auto n: nodes) {
            int deg = n->get_deg();
            int curr_deg = n->get_d()->curr_deg();
            if(curr_deg % 2 != 0) { //uneven non-black node
                u = n;
                if(curr_deg != deg) { //uneven and grey
                    k_old = u->get_d()->get_from_o(); //get index of a_old in incidence arr
                }
                break;
            } else if(curr_deg > 0 && curr_deg != deg) { //grey node
                u = n;
                break;
            } else if(curr_deg == deg) { //white node
                u = n;
            }

            //else: black node, we don't do anything
        }
        //all nodes left are black, loop ends
        if(u == nullptr) break;

        /**
         * CrossLinkedInt* k_first = leave(u);
         */
        extend(u);
    }
}

Node *Graph::get_node(int u) {
    return nodes.at(u);
}

void Graph::extend(Node *e) {

}



