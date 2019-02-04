#ifndef SEALIB_GRAPH_UNDIRECTEDGRAPH_H_
#define SEALIB_GRAPH_UNDIRECTEDGRAPH_H_

#include <sealib/graph/graph.h>
#include <sealib/graph/node.h>
#include <vector>
#include <tuple>

namespace Sealib {
/**
 * A undirected graph G = (V, E) with nodes V = [0, n - 1].
 * The graph stores its edges V inside adjacency arrays containing
 * cross pointers.
 * @complexity (O((n + m) log n)) bits Stores a undirected graph with n vertices and m edges using (n + 4m) log n bits.
 * @author Johannes Meintrup
 */
class UndirectedGraph : public Graph {
 public:
    /**
     * Created a new graph object with the nodes provided by the nodes_ vector
     * @param nodes_ vector of nodes (const ref)
     */
    explicit UndirectedGraph(std::vector<ExtendedNode> const &nodes_) : nodes(nodes_) {}

    /**
     * Created a graph with the specified order and without any edges.
     * @param order - order of the graph
     */
    explicit UndirectedGraph(uint order) : nodes(order) {}

    /**
     * Adds a new node to the graph
     * @param const ref of node to be added
     */
    void addNode(ExtendedNode const &node) { nodes.emplace_back(node); }

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array.
     * @return Reference to Node in the nodes array.
     */
    const ExtendedNode &getNode(uint u) const { return nodes[u]; }
    ExtendedNode &getNode(uint u) { return nodes[u]; }

    /**
    * Returns the degree of the node that u points at.
    * @param u Vertex u 
    * @return Returns d that is the degree of node v.
    */
    uint deg(uint u) const override { return nodes[u].getDegree(); }

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return the k-th neighbor of u
    */
    uint head(uint u, uint k) const override { return nodes[u].getAdj()[k].first; }

    /**
    * @return order of the graph, i.e, the total number of vertices.
    */
    uint getOrder() const override { return nodes.size(); }

    /**
     * Get the cross index of the given edge.
     * @param u node
     * @param k outgoing edge
     * @return index of the edge k' in v that points back to u
     */
    uint mate(uint u, uint k) const { return nodes[u].getAdj()[k].second; }

 private:
    std::vector<ExtendedNode> nodes;
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_UNDIRECTEDGRAPH_H_
