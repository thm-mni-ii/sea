#ifndef SEALIB_BASICGRAPH_H_
#define SEALIB_BASICGRAPH_H_

#include <sealib/graph.h>
#include <sealib/node.h>
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
class BasicGraph : public Graph {
 private:
    std::vector<Sealib::Node> nodes;

 public:
    /**
     * Creates a graph out of its nodes.
     * @param _nodes Array of nodes.
     * @param _order Order of the graph (equals the length of the nodes array).
     */
    BasicGraph(Node *nodes_, unsigned int order_);

    /**
     * Created a new graph object with the nodes provided by the nodes_ vector
     * @param nodes_ vector of nodes (ref)
     */
    explicit BasicGraph(const std::vector<Sealib::Node> &nodes_);

    /**
     * Created a new empty graph object with no nodes
     */
    BasicGraph();

    ~BasicGraph() override = default;

    /**
     * Created a graph with the specified order and without any edges.
     * @param order - order of the graph
     */
    explicit BasicGraph(unsigned int order);

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
    const Node &getNode(unsigned int u) const;

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array.
     * @return Pointer to the Node in the nodes array.
     */
    Node &getNode(unsigned int u);

    /**
    * Returns the degree of the node that u points at.
    * @param u Vertex u 
    * @return Returns d that is the degree of node v.
    */
    unsigned int getNodeDegree(unsigned int u) const override;

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns v that is the k-th neighbor of u.
    */
    unsigned int head(unsigned int u, unsigned int k) const override;

    /**
    * Returns the mate of the k-th arc out of u.
    * Which is the k'-th arc out of u'.
    * u' = head of (u, k)
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns (u', k')
    */
    std::tuple<unsigned int, unsigned int> mate(unsigned int u, unsigned int k) const;

    /**
    * @return Returns the order of the graph, i.e, the total number of vertices.
    */
    unsigned int getOrder() const override;
};
}  // namespace Sealib
#endif  // SEALIB_BASICGRAPH_H_
