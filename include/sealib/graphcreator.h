//
// Created by jmeintrup on 14.03.18.
//

#ifndef SEA_GRAPHCREATOR_H
#define SEA_GRAPHCREATOR_H

#include "graph.h"

/**
 * Static class which serves as a collection of functions to create graph objects from different inputs.
 */
class GraphCreator{
public:
    /**
     * Static function to create a a graph object from an adjacency matrix.
     *
     * example matrix with order 3:
     *
     * n         0       1       2
     *      **************************
     *  0   *    0   *   1   *   1   *
     *      **************************
     *  1   *    1   *   0   *   2   *
     *      **************************
     *  2   *    0   *   2   *   1   *
     *      **************************
     * @param adj_matrix NxN adjcacency matrix representation of the graph.
     * @param _order Order of the graph, order equals the number of nodes.
     */
    static Graph* createGraphFromAdjacencyMatrix(unsigned int **adjMatrix, unsigned int order);
};



#endif //SEA_GRAPHCREATOR_H
