#ifndef SEALIB_GRAPH_DIRECTEDGRAPH_H_
#define SEALIB_GRAPH_DIRECTEDGRAPH_H_

#include <sealib/graph/graph.h>
#include <sealib/graph/node.h>
#include <vector>

namespace Sealib {
/**
 * A directed graph G = (V, E) with nodes V = [0, n - 1].
 * The graph stores its edges V inside adjacency arrays.
 * Complexity: (O((n + m) log n)) bits Stores a directed graph with n vertices
 * and m edges using (n + m) log n bits.
 */
class DirectedGraph : public Graph {
 public:
    /**
     * Creates a new graph object with the nodes provided by the nodes_ vector
     * @param _nodes vector of nodes
     */
    explicit DirectedGraph(std::vector<SimpleNode> _nodes)
        : nodes(std::move(_nodes)) {}

    /**
     * Created a graph with the specified order and without any edges.
     * @param order order of the graph
     */
    explicit DirectedGraph(uint64_t order) : nodes(order) {}

    /**
     * Adds a new node to the graph
     * @param node node to be added
     */
    void addNode(SimpleNode const &node) { nodes.emplace_back(node); }

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array
     * @return Reference to the Node in the nodes array (const or non-const
     * version)
     */
    SimpleNode const &getNode(uint64_t u) const { return nodes[u]; }
    SimpleNode &getNode(uint64_t u) { return nodes[u]; }

    /**
     * Returns the degree of the node that u points at.
     * @param u Vertex number
     * @return degree of node u
     */
    uint64_t deg(uint64_t u) const override { return nodes[u].getDegree(); }

    /**
     * Returns the vertex v that u points at with its k-th edge.
     * @param u Vertex u
     * @param k index in the adjacency vector of node u
     * @return the k-th neighbor of u
     */
    uint64_t head(uint64_t u, uint64_t k) const override {
        return nodes[u].getAdj()[k];
    }

    /**
     * @return the order of the graph, i.e, the total number of
     * vertices.
     */
    uint64_t getOrder() const override {
        return static_cast<uint64_t>(nodes.size());
    }

 private:
    std::vector<SimpleNode> nodes;
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_DIRECTEDGRAPH_H_
