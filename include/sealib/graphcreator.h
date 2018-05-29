#ifndef SEALIB_GRAPHCREATOR_H_
#define SEALIB_GRAPHCREATOR_H_

#include "sealib/graph.h"

/**
 * Static class that serves as a collection of functions to create graph objects from different inputs.
 * @author Johannes Meintrup
 */
class GraphCreator {
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
     * @param adj_matrix NxN adjacency matrix representation of the graph.
     * @param _order Order of the graph, order equals the number of nodes.
     */
    static Graph *createGraphFromAdjacencyMatrix(unsigned int **adjMatrix, unsigned int order);
};

#endif  // SEALIB_GRAPHCREATOR_H_
