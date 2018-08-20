#ifndef SEALIB_GRAPH_H_
#define SEALIB_GRAPH_H_

#include "sealib/node.h"

namespace Sealib {
/**
 * A undirected graph G = (V, E) with nodes V = [0, n - 1].
 * The graph stores its edges V inside adjacency arrays containing
 * cross pointers.
 * @complexity (O((n + m) log n)) bits Stores a undirected graph with n vertices and m edges using (n + 4m) log n bits.
 * @author Johannes Meintrup
 */
class Basicgraph : Graph {
 private:
    Node *nodes;
    unsigned int order;

 public:
    /**
     * Creates a graph out of its nodes.
     * @param _nodes Array of nodes.
     * @param _order Order of the graph (equals the length of the nodes array).
     */
    Graph(Node *_nodes, unsigned int _order);

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array.
     * @return Pointer to the Node in the nodes array.
     */
    Node *getNode(unsigned int u);

    /**
    * Returns the degree of the node that v points at.
    * @param v Vertex v
    * @return Returns d that is the degree of node v.
    */
		unsigned int getNodeDegree(unsigned int v) override;

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns v that is the k-th neighbor of u.
    */
    unsigned int head(unsigned int u, unsigned int k) override;

    /**
    * @return Returns the order of the graph, i.e, the total number of vertices.
    */
    unsigned int getOrder() override;
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_H_
