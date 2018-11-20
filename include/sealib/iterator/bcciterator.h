#ifndef SEALIB_ITERATOR_BCCITERATOR_H_
#define SEALIB_ITERATOR_BCCITERATOR_H_

#include <utility>
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/edgemarker.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {
/**
 * Iterator used to output all vertices in a biconnected component (BCC) of an
 * undirected graph G. First, init() the iterator; then, call start(x,y) with an
 * edge {x,y} in G to set the starting point of the BCC iterator. After that,
 * call more() and next() alternately to retrieve all the vertices of the BCC.
 */
class BCCIterator : Iterator<std::pair<uint, uint>> {
 public:
    /**
     * Create a new BCC iterator from a given undirected graph.
     * @param g undirected graph G=(V,E)
     */
    explicit BCCIterator(UndirectedGraph *g);

    /**
     * Create a new BCC iterator from an existing EdgeMarker object.
     * @param e EdgeMarker that holds the markings and edge types for a graph G
     * (e.g. created by CutVertexIterator)
     */
    explicit BCCIterator(EdgeMarker *e);

    ~BCCIterator();

    /**
     * Initialize the iterator. If no external EdgeMarker was given, identifies
     * tree edges and marks them.
     */
    void init() override;

    /**
     * Set the start edge for the iterator.
     * @param u node in G (ancestor of v)
     * @param v node in G (descendant of u)
     */
    void start(uint u, uint v);

    /**
     * Runs a DFS as long as there are more vertices in the BCC.
     * @return true if there is another vertex in the BCC
     */
    bool more() override;

    /**
     * @return the next vertex or edge in the BCC: vertices in the form
     * (u,INVALID), edges in the form (u,v). The order of output is the
     * following: 1) node u; 2) edge (u,v); 3) back edges to v (if any); 4)
     * vertex v.
     * (INVALID is a global constant defined in _types.h.)
     */
    std::pair<uint, uint> next() override;

    /**
     * Moves the EdgeMarker object out of this instance to reuse it somewhere
     * else.
     * @return the used EdgeMarker
     */
    inline EdgeMarker getEdgeMarker() { return std::move(*e); }

 private:
    UndirectedGraph *g;
    uint n;
    EdgeMarker *e;
    CompactArray color;
    StaticSpaceStorage parent;

    std::pair<uint, uint> startEdge;
    uint node, edge;
    uint latestNode;
    bool endOnNextNode = false;
    bool oneMoreOutput = false;
    bool outputtingBackEdges = false;
    bool firstNode;

    bool externalEdgeMarker;
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_BCCITERATOR_H_
