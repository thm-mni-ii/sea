#ifndef SEALIB_GRAPH_GRAPHCREATOR_H_
#define SEALIB_GRAPH_GRAPHCREATOR_H_

#include <sealib/graph/compactgraph.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/undirectedgraph.h>
#include <memory>
#include <random>

namespace Sealib {
/**
 * Collection of functions to create directed and undirected graphs.
 */
class GraphCreator {
 public:
    /**
     * Create a graph from an adjacency matrix.
     * @param adjMatrix NxN adjacency matrix of the graph: M[i,j] = number of edges from i to j
     * @param order Order of the graph (number of nodes)
     * @return the generated graph
     * @author Johannes Meintrup
     */
    static UndirectedGraph createGraphFromAdjacencyMatrix(uint32_t **adjMatrix,
                                                          uint32_t order);

    static UndirectedGraph *createGraphPointerFromAdjacencyMatrix(
        uint32_t **adjMatrix, uint32_t order);

    static std::shared_ptr<UndirectedGraph>
    createSharedGraphFromAdjacencyMatrix(uint32_t **adjMatrix, uint32_t order);

    /**
     * Create a random k-regular graph, i.e., a graph with a fixed number
     * of neighbours per node.
     * @param order number of nodes the graph should contain
     * @param k the degree of each node; the edges will go to any
     * random node
     * @return the resulting graph G (n = order, m = k*order)
     * @author Simon Heuser
     */
    static DirectedGraph createRandomKRegularGraph(uint32_t order, uint32_t k);

    /**
     * Create a random k-regular undirected graph.
     * (Takes very long for larger graphs!)
     * @param order number of nodes to generate
     * @param k outgoing edges per node
     * @return the resulting undirected graph
     * @author Simon Heuser
     */
    static UndirectedGraph createRandomKRegularUndirectedGraph(
        uint32_t order, uint32_t outdegreePerNode);

    /**
     * Create a completely random graph with a given number of nodes. Each node
     * will have outgoing edges to at most n other nodes.
     * @param order the number of nodes the graph should contain
     * @return the resulting graph: n = order, m = O(n^2)
     * @author Simon Heuser
     */
    static DirectedGraph createRandomGenerated(uint32_t order);

    /**
     * Generate a random undirected graph. Each node will have at least 5
     * connections to other nodes.
     * @param order number of nodes
     * @return the resulting undirected graph: n = order, m = O(n)
     * @author Simon Heuser
     */
    static UndirectedGraph createRandomGeneratedUndirected(uint32_t order);

    /**
     * Create a random "imbalanced" graph, which contains a handful of very
     * large nodes.
     * @param order number of nodes
     * @return the resulting graph: some nodes have a very large degree (they
     * are "big")
     */
    static DirectedGraph createRandomImbalanced(uint32_t order);

    /**
     *  Generate a Gilbert graph G(n,p) in standard representation with n nodes
     *  and an occurence of every possible edge with a probability of p (0 < p < 1)
     *  Complexity: O(n^2)
     *	@param n order of the graph
     *	@param p probability of an edge
     *	@param gen engine to generate a pseudo random sequence of numbers
     *  @author Simon Schniedenharn
     */
    static CompactGraph generateGilbertGraph(uint32_t n, double p,
                                             std::mt19937_64 *gen);

    static uint *fastGraphGeneration(uint n, uint mPern);

    /**
     * Create a random bipartite graph with the vertex sets V1 and V2 and the probability p for each possible edge to occur.
     * @param order1 Number of vertices in V1
     * @param order2 Number of vertices in V2
     * @param p Edge probability
     * @param seed Seed for the random number generator
     * @author Johannes Meintrup
     */
    static std::unique_ptr<UndirectedGraph>
    generateRandomBipartiteUndirectedGraph(uint32_t order1, uint32_t order2,
                                           double p, uint32_t seed);

    /**
     * Create an undirected windmill graph (m complete graphs of order n joined
     * together at a common vertex).
     * @param order order of each windmill part (n > 1)
     * @param count number of windmill parts (m)
     * @return the generated windmill graph W_n^m
     * @author Simon Heuser
     */
    static UndirectedGraph createWindmill(uint32_t order, uint32_t count);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_GRAPHCREATOR_H_
