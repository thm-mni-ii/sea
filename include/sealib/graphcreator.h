#ifndef SEALIB_GRAPHCREATOR_H_
#define SEALIB_GRAPHCREATOR_H_

#include <sealib/basicgraph.h>
#include <memory>

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
  static Sealib::BasicGraph *createGraphFromAdjacencyMatrix(
      unsigned int **adjMatrix, unsigned int order);

  /**
   * Create a random graph with a fixed number of neighbours per node.
   * @param order number of nodes the graph should contain
   * @param degreePerNode the degree of each node; the edges will go to any
   * random node
   * @return the resulting graph G: n = order, m = degreePerNode * order
   * @author Simon Heuser
   */
  static Sealib::BasicGraph *createRandomFixed(unsigned int order,
                                               unsigned int degreePerNode);

  /**
   * Create a completely random graph with a given number of nodes. Each node
   * will have outgoing edges to at most n other nodes.
   * @param order the number of nodes the graph should contain
   * @return the resulting graph: n = order, m = O(n^2)
   * @author Simon Heuser
   */
  static Sealib::BasicGraph *createRandomGenerated(unsigned int order);

  /**
    *Create a random "imbalanced" graph, which contains a handful of very large
    *nodes.
    *@param order number of nodes
    *@return the resulting graph: some nodes have a very large degree (they are
    *"big")
    */
  static Sealib::BasicGraph *createRandomImbalanced(unsigned int order);

  /**
   * Creates a random graph with every possible edge occuring with a chance of
   * p.
   * @param order of the graph
   * @param p chance that an edge occurs, 0.0 < p < 1.0
   * @param seed for the rng
   * @return unique_ptr to the graph
   */
  static std::unique_ptr<Sealib::BasicGraph> generateRandomBasicGraph(
      unsigned int order, double p, unsigned int seed);

  /**
  * Creates a random bipartite graph with every possible edge occuring with a
  * chance of p.
  * @param n1 - number of nodes in the first part of the graph
  * @param n2 - number of nodes in the second part of the graph
  * @param p chance that an edge occurs, 0.0 < p < 1.0
  * @param seed for the rng
  * @return unique_ptr to the graph
  */
  static std::unique_ptr<Sealib::BasicGraph> generateRandomBipartiteBasicGraph(
      unsigned int n1, unsigned int n2, double p, unsigned int seed);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPHCREATOR_H_
