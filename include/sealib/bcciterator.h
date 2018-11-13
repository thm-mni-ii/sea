#ifndef SEALIB_BCCITERATOR_H_
#define SEALIB_BCCITERATOR_H_
#include <memory>
#include "sealib/basicgraph.h"
#include "sealib/edgemarker.h"
#include "sealib/iterator.h"

namespace Sealib {
/**
 * Iterator used to output all vertices in a biconnected component (BCC) of an
 * undirected graph G. First, init() the iterator; then, call start(x,y) with an
 * edge {x,y} in G to set the starting point of the BCC iterator. After that,
 * call more() and next() alternately to retrieve all the vertices of the BCC.
 */
class BCCIterator : Iterator<uint> {
 public:
    /**
     * Create a new BCC iterator from a given undirected graph.
     * @param g undirected graph G=(V,E)
     */
    explicit BCCIterator(BasicGraph *g);

    /**
     * Create a new BCC iterator from an existing EdgeMarker object.
     * @param e EdgeMarker that holds the markings and edge types for a graph G
     * (e.g. created by CutVertexIterator)
     */
    explicit BCCIterator(EdgeMarker *e);

    ~BCCIterator();

    /**
     * Initialize the iterator. If no external EdgeMarker was given, identifies tree edges and marks them.
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
     * @return the next vertex in the BCC
     */
    uint next() override;

 private:
    BasicGraph *g;
    uint n;
    EdgeMarker *e;
    CompactArray color;
    StaticSpaceStorage parent;

    Pair startEdge;
    uint node, edge;
    bool endOnNextStep = false;

    bool externalEdgeMarker;
};
}  // namespace Sealib

#endif  // SEALIB_BCCITERATOR_H_
