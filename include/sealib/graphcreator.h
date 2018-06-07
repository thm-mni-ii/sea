#ifndef SEA_GRAPHCREATOR_H
#define SEA_GRAPHCREATOR_H

#include <string>
#include "sealib/graph.h"


/**
 * Static class that serves as a collection of functions to create graph objects from different inputs.
 * @author Johannes Meintrup
 */
namespace Sealib {
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
    static void createGraphFromAdjacencyMatrix(const std::vector<std::vector<unsigned int>>& adjMatrix, unsigned int order, Sealib::Graph &graph);

    static void randomEulerianOgdfGraph(unsigned int nodeMax, unsigned int edgeMax, Graph &g);
};
}  // namespace Sealib
#endif  // SEA_GRAPHCREATOR_H
