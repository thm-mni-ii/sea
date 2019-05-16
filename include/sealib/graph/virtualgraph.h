#ifndef SEALIB_GRAPH_VIRTUALGRAPH_H_
#define SEALIB_GRAPH_VIRTUALGRAPH_H_
#include <vector>
#include "sealib/collection/staticspacestorage.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/dictionary/raggeddictionary.h"
#include "sealib/graph/undirectedgraph.h"

namespace Sealib {

/**
 * A virtual subgraph of a given undirected graph. Vertices and
 * edges can be deleted from the virtual graph. Adding new edges is only
 * supported in a rudimentary way (should be implemented in the using algorithm
 * if necessary).
 *
 * @author Simon Heuser
 */
class VirtualGraph : UndirectedGraph {
 public:
    /**
     * Creates a new virtual graph.
     * @param g Graph
     */
    explicit VirtualGraph(UndirectedGraph const &g);

    /**
     * Returns the degree of the given vertex.
     * @param u Vertex u
     * @return number of neighbours for vertex u
     */
    uint64_t deg(uint64_t u) const override;
    [[gnu::hot]] uint64_t deg(uint64_t u, bool artificialEdges) const;

    /**
     * Returns the vertex v that u points at with its k-th edge.
     * @param u vertex u
     * @param k index in the adjacency vector of node u
     * @return k-th neighbor of u
     */
    [[gnu::hot]] uint64_t head(uint64_t u, uint64_t k) const override;

    /**
     * Get the cross index of the given outgoing edge.
     * @param u vertex
     * @param k outgoing edge
     * @return index k' of the edge leading back to u
     */
    uint64_t mate(uint64_t u, uint64_t k) const override;

    /**
     * Get the original edge index of the given outgoing edge.
     * @param u vertex
     * @param k outgoing edge index of the virtual graph
     * @return k^0: the index of the edge in the original adjacency list of u
     */
    uint64_t index(uint64_t u, uint64_t k) const;

    /**
     * @return Returns the order of the graph, i.e, the total number of
     * vertices.
     */
    uint64_t getOrder() const override;

    /**
     * Get an iterator over all present vertices.
     * @return CD iterator over the vertices present in the graph
     */
    ChoiceDictionaryIterator vertices() const;

    /**
     * Check if the given vertex is still present in the virtual graph.
     * @param u vertex to check
     * @return true if vertex u has not been removed (yet)
     */
    bool hasVertex(uint64_t u) const;

    /**
     * Remove the given vertex from the virtual graph.
     * @param u vertex to be removed
     */
    void removeVertex(uint64_t u);

    /**
     * Adds a virtual edge between the given vertices.
     * (Note: Only up to O(n/log(n)) virtual edges may be present at any time.)
     * (Attention: Only add an edge for a vertex if at least 1 of its edges has
     * been removed previously.)
     * EFFICIENCY: O(log(log(n))) time
     * @param u endpoint of the virtual edge
     * @param v endpoint of the virtual edge
     */
    void addEdge(uint64_t u, uint64_t v);

    /**
     * Remove the given virtual edge {u,v} from the virtual graph.
     * EFFICIENCY: O(log(log(n))) time
     * @param u first endpoint of the virtual edge
     * @param v second endpoint of the virtual edge
     */
    void removeEdge(uint64_t u, uint64_t v);

    uint64_t byteSize() const {
        return presentVertices.capacity() / 8 + virtualEdges.byteSize();
    }

 private:
    UndirectedGraph const &g;
    uint64_t n;
    std::vector<bool> presentVertices;
    RaggedPairDictionary virtualEdges;
};

}  // namespace Sealib

#endif  // SEALIB_GRAPH_VIRTUALGRAPH_H_
