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
     * @param adjMatrix NxN adjacency matrix of the graph: M[i,j] = number of
     * edges from i to j
     * @param order Order of the graph (number of nodes)
     * @return the generated graph
     * @author Johannes Meintrup
     */
    static UndirectedGraph createFromAdjacencyMatrix(uint64_t **adjMatrix,
                                                     uint64_t order);

    static UndirectedGraph *createPointerFromAdjacencyMatrix(
        uint64_t **adjMatrix, uint64_t order);

    static std::shared_ptr<UndirectedGraph>
    createSharedPointerFromAdjacencyMatrix(uint64_t **adjMatrix,
                                           uint64_t order);

    /**
     * Create a random directed graph with a fixed number of outneighbours per
     * node.
     * @param order number of nodes the graph should contain
     * @param k the degree of each node; the edges will go to any random node
     * @return the resulting graph G (n = order, m = k*order)
     * @author Simon Heuser
     */
    static DirectedGraph kOutdegree(uint64_t order, uint64_t k);

    /**
     * Create a random k-regular graph.
     * Complexity: O(n*k) time
     * @param order number of nodes to generate (should be an even number!)
     * @param k the degree of each node
     * @return the resulting undirected graph G (n = order, m = k*order)
     * (Note: if order is an odd number, G will have order+1 vertices)
     * @author Simon Heuser
     */
    static UndirectedGraph kRegular(uint64_t order, uint64_t k);

    /**
     * Create a random sparseDirected graph with a given number of nodes. Each
     * node
     * will have outgoing edges to at most log(n) other nodes.
     * @param order the number of nodes the graph should contain
     * @return the resulting graph: n = order, m = O(n*log(n))
     * @author Simon Heuser
     */
    static DirectedGraph sparseDirected(uint64_t order);

    /**
     * Generate a random undirected graph. Each node will be adjacent to O(log
     * n) nodes.
     * @param order number of nodes
     * @return the resulting undirected graph: n = order, m = O(n*log(n))
     * @author Simon Heuser
     */
    static UndirectedGraph sparseUndirected(uint64_t order);

    /**
     * Create a random "imbalanced" graph, which contains a handful of very
     * large nodes.
     * @param order number of nodes
     * @return the resulting graph: some nodes have a very large degree (they
     * are "big")
     */
    static DirectedGraph imbalanced(uint64_t order);

    /**
     *  Generate a Gilbert graph G(n,p) in standard representation with n nodes
     *  and an occurence of every possible edge with a probability of p (0 < p <
     *1)
     *  Complexity: O(n^2)
     *	@param n order of the graph
     *	@param p probability of an edge
     *	@param gen engine to generate a pseudo random sequence of numbers
     *  @author Simon Schniedenharn
     */
    static CompactGraph generateGilbertGraph(uint64_t n, double p,
                                             std::mt19937_64 *gen);

    static uint64_t *fastGraphGeneration(uint64_t n, uint64_t mPern);

    /**
     * Create a random bipartite graph with the vertex sets V1 and V2 and the
     * probability p for each possible edge to occur.
     * @param order1 Number of vertices in V1
     * @param order2 Number of vertices in V2
     * @param p Edge probability
     * @param seed Seed for the random number generator
     * @author Johannes Meintrup
     */
    static std::unique_ptr<UndirectedGraph> randomBipartite(uint64_t order1,
                                                            uint64_t order2,
                                                            double p,
                                                            uint64_t seed);

    /**
     * Create an undirected windmill graph (m complete graphs of order n joined
     * together at a common vertex).
     * @param order order of each windmill part (n > 1)
     * @param count number of windmill parts (m)
     * @return the generated windmill graph W_n^m (|V| = n*m-m+1, |E| =
     * n*m*(n-1))
     * @author Simon Heuser
     */
    static UndirectedGraph windmill(uint64_t order, uint64_t count);

    /**
     * Create a transpose of the input graph. In a transposed graph, all the
     * edge directions are reversed.
     * @param g directed graph
     * @return transposed input graph
     */
    static DirectedGraph transpose(DirectedGraph const &g);

    /**
     * Create a triangulated graph with the given number of vertices. A number
     * of triangles is joined together so that the resulting graph is
     * biconnected and maximally outerplanar.
     * @param order number of vertices
     * @return triangulated graph (n=order, m=2*n-3)
     */
    static UndirectedGraph triangulated(uint64_t order);

    /**
     * Create the cycle graph C^n.
     * @param order number of vertices
     * @param chords number of chords inside the cycle
     * @return outerplanar cycle graph G (n=order, m=order+chords)
     */
    static UndirectedGraph cycle(uint64_t order, uint64_t chords = 0);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_GRAPHCREATOR_H_
