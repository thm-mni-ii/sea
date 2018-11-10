#ifndef SEALIB_GRAPH_GRAPHCREATOR_H_
#define SEALIB_GRAPH_GRAPHCREATOR_H_

#include <sealib/graph/compactgraph.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/undirectedgraph.h>
#include <memory>
#include <random>

namespace Sealib {
/**
 * Static class that serves as a collection of functions to create graph objects
 * from different inputs.
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
    static UndirectedGraph createGraphFromAdjacencyMatrix(uint32_t **adjMatrix,
                                                          uint32_t order);

    static UndirectedGraph *createGraphPointerFromAdjacencyMatrix(
        uint32_t **adjMatrix, uint32_t order);

    static std::shared_ptr<UndirectedGraph>
    createSharedGraphFromAdjacencyMatrix(uint32_t **adjMatrix, uint32_t order);

    /**
     * Create a random graph with a fixed number of neighbours per node.
     * @param order number of nodes the graph should contain
     * @param degreePerNode the degree of each node; the edges will go to any
     * random node
     * @return the resulting graph G (n = order, m = degreePerNode*order)
     * @author Simon Heuser
     */
    static DirectedGraph createRandomFixed(uint32_t order,
                                           uint32_t degreePerNode);

    /**
     * Create a completely random graph with a given number of nodes. Each node
     * will have outgoing edges to at most n other nodes.
     * @param order the number of nodes the graph should contain
     * @return the resulting graph: n = order, m = O(n^2)
     * @author Simon Heuser
     */
    static DirectedGraph createRandomGenerated(uint32_t order);

    /**
     * Create a random "imbalanced" graph, which contains a handful of very large nodes.
     * @param order number of nodes
     * @return the resulting graph: some nodes have a very large degree (they are "big")
     */
    static DirectedGraph createRandomImbalanced(uint32_t order);

    /**
     *  Generates a Gilbert graph G(n,p) in standard representation with n nodes
     *  and an occurence of every possible edge with a probability of
     *  0 < p < 1
     *  Complexity = O(n^2)
     *
     *	@param numNodes order of the graph
     *	@param p probability of an edge
     *	@param gen engine to generate a pseudo random sequence of numbers
     */
    static CompactGraph generateGilbertGraph(uint32_t n, double p,
                                             std::mt19937_64 *gen);

    static uint *fastGraphGeneration(uint n, uint mPern);

    static std::unique_ptr<UndirectedGraph>
    generateRandomBipartiteUndirectedGraph(uint32_t order1, uint32_t order2,
                                           double p, uint32_t seed);

    static std::pair<UndirectedGraph *, uint32_t> createRandomUndirected(
        uint32_t order, uint32_t approxDegree);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_GRAPHCREATOR_H_
