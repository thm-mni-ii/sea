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
     * Created a new graph object with the nodes provided by the nodes_ vector
     * @param nodes_ vector of nodes (ref)
     */
    explicit DirectedGraph(std::vector<SimpleNode> const &_nodes)
        : nodes(_nodes) {}

    /**
     * Created a graph with the specified order and without any edges.
     * @param order - order of the graph
     */
    explicit DirectedGraph(uint order) : nodes(order) {}

    /**
     * Adds a new node to the graph
     * @param const ref of node to be added
     */
    void addNode(SimpleNode const &node) { nodes.emplace_back(node); }

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array
     * @return Reference to the Node in the nodes array (const or non-const
     * version)
     */
    SimpleNode const &getNode(uint u) const { return nodes[u]; }
    SimpleNode &getNode(uint u) { return nodes[u]; }

    /**
     * Returns the degree of the node that u points at.
     * @param u Vertex number
     * @return degree of node u
     */
    uint deg(uint u) const override { return nodes[u].getDegree(); }

    /**
     * Returns the vertex v that u points at with its k-th edge.
     * @param u Vertex u
     * @param k index in the adjacency vector of node u
     * @return the k-th neighbor of u
     */
    uint head(uint u, uint k) const override { return nodes[u].getAdj()[k]; }

    /**
     * @return the order of the graph, i.e, the total number of
     * vertices.
     */
    uint getOrder() const override { return static_cast<uint>(nodes.size()); }

 private:
    std::vector<SimpleNode> nodes;
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_DIRECTEDGRAPH_H_
