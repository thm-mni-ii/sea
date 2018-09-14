#ifndef SEALIB_GRAPHCREATOR_H_
#define SEALIB_GRAPHCREATOR_H_

#include <sealib/basicgraph.h>
#include <memory>

namespace Sealib {
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
    static BasicGraph createGraphFromAdjacencyMatrix(unsigned int **adjMatrix, unsigned int order);

    /**
     * Creates a random graph with every possible edge occuring with a chance of p.
     * @param order of the graph
     * @param p chance that an edge occurs, 0.0 < p < 1.0
     * @param seed for the rng
     * @return unique_ptr to the graph
     */
    std::unique_ptr<Sealib::BasicGraph> generateRandomBasicGraph(
        unsigned int order,
        double p,
        unsigned int seed);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPHCREATOR_H_
