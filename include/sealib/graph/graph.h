#ifndef SEALIB_GRAPH_GRAPH_H_
#define SEALIB_GRAPH_GRAPH_H_
#include <cstdint>
#include <sealib/_types.h>

namespace Sealib {
/**
 * Graph is a virtual class that provides an interface for graph traversing
 * algorithms.
 * @Author Simon Schniedenharn
 */
class Graph {
 public:
    /**
    * Returns the degree of the node that v points at.
    * @param v Vertex v
    * @return Returns d that is the degree of node v.
    */
    virtual uint deg(uint v) const = 0;

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns v that is the k-th neighbor of u.
    */
    virtual uint head(uint u, uint k) const = 0;

    /**
    * @return Returns the order of the graph, i.e, the total number of vertices.
    */
    virtual uint getOrder() const = 0;
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_GRAPH_H_
