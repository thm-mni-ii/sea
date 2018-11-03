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
 private:
    std::vector<Sealib::Node> nodes;

 public:
    /**
     * Creates a graph out of its nodes.
     * @param _nodes Array of nodes.
     * @param _order Order of the graph (equals the length of the nodes array).
     */
    UndirectedGraph(Node *nodes_, uint32_t order_);

    /**
     * Created a new graph object with the nodes provided by the nodes_ vector
     * @param nodes_ vector of nodes (ref)
     */
    explicit UndirectedGraph(const std::vector<Sealib::Node> &nodes_);

    /**
     * Created a new empty graph object with no nodes
     */
    UndirectedGraph();

    /**
     * Created a graph with the specified order and without any edges.
     * @param order - order of the graph
     */
    explicit UndirectedGraph(uint32_t order);

    /**
     * Adds a new node to the graph
     * @param const ref of node to be added
     */
    void addNode(const Sealib::Node& node);

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array.
     * @return Pointer to the Node in the nodes array.
     */
    const Node &getNode(uint32_t u) const;

    ~UndirectedGraph() override = default;

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array.
     * @return Pointer to the Node in the nodes array.
     */
    Node &getNode(uint32_t u);

    /**
    * Returns the degree of the node that u points at.
    * @param u Vertex u 
    * @return Returns d that is the degree of node v.
    */
    uint32_t getNodeDegree(uint32_t u) const override;

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns v that is the k-th neighbor of u.
    */
    uint32_t head(uint32_t u, uint32_t k) const override;

    /**
    * @return Returns the order of the graph, i.e, the total number of vertices.
    */
    uint32_t getOrder() const override;

    std::tuple<uint32_t, uint32_t> mate(uint32_t u, uint32_t k) const;
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_UNDIRECTEDGRAPH_H_
