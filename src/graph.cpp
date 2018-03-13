//
// Created by jmeintrup on 06.03.18.
//

#include <iostream>
#include <sealib/trailstructure.h>
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
    TrailStructure* trailStructures[order];
    for(int i = 0; i < order; i++) {
        trailStructures[i] = new TrailStructure();
        for(int j = 0; j < nodes.at(i)->get_deg(); j++) {
            trailStructures[i]->add_to_u(new CrossLinkedInt(i));
        }
    }
}