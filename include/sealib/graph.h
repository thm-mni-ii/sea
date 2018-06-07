#ifndef SEA_GRAPH_H
#define SEA_GRAPH_H

#include "sealib/node.h"


/**
 * A undirected graph G = (V, E) with nodes V = [0, n - 1].
 * The graph stores its edges V inside adjacency arrays containing
 * cross pointers.
 * @complexity (O((n + m) log n)) bits Stores a undirected graph with n vertices and m edges using (n + 4m) log n bits.
 * @author Johannes Meintrup
 */
namespace Sealib {
class Graph {
private:
    std::vector<Sealib::Node> nodes;

public:
    /**
     * empty graph.
     */
    Graph();

    /**
     * Getter for a specific node in the nodes array.
     * @param u index in the nodes array.
     * @return Pointer to the Node in the nodes array.
     */
    const Sealib::Node &getNode(unsigned int u) const;

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns v that is the k-th neighbor of u.
    */
    unsigned int head(unsigned int u, unsigned int k) const ;

    /**
    * @return Returns the order of the graph, i.e, the total number of vertices.
    */
    unsigned int getOrder() const;

    /**
     * Sets the nodes of the graph
     * @param nodes_ - Node vector
     */
    void setNodes(std::vector<Sealib::Node> nodes_);
};
}

#endif  // SEA_GRAPH_Hs