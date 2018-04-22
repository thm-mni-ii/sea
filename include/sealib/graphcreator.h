#ifndef SEA_GRAPHCREATOR_H
#define SEA_GRAPHCREATOR_H

#include "graph.h"
#include <string>

/**
 * Static class that serves as a collection of functions to create graph objects from different inputs.
 * @author Johannes Meintrup
 */
class GraphCreator {
public:
    /**
     * Creates a graph from a dotfile defined by the fileName string.
     *
     * The file should be in the following format:
     * Graph G {
     *  0 -- 1
     *  ...
     *  0 -- n
     *  ...
     *  n -- 0
     *  ...
     *  n -- k
     *  ...
     * }
     * @param fileName fileName of the .dot file
     */
    static Graph *createGraphFromDotFile(std::string fileName);

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

#endif //SEA_GRAPHCREATOR_H
