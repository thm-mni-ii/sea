#ifndef SEALIB_ITERATOR_BCCITERATOR_H_
#define SEALIB_ITERATOR_BCCITERATOR_H_

#include <memory>
#include <utility>
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/edgemarker.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {
/**
 * Iterator used to output all vertices in a biconnected component (BCC) of an
 * undirected graph G. First, call init(x) to set the starting point of the BCC
 * iterator. After that, call more() and next() alternately to retrieve all the
 * vertices of the BCC.
 */
class BCCIterator : Iterator<std::pair<uint64_t, uint64_t>> {
 public:
    /**
     * Create a new BCC iterator from a given undirected graph.
     * @param g undirected graph G=(V,E)
     */
    explicit BCCIterator(UndirectedGraph const &g);

    /**
     * Create a new BCC iterator from a given edge marker (allows recycling).
     * @param e shared pointer to an EdgeMarker
     */
    explicit BCCIterator(std::shared_ptr<EdgeMarker> e);

    /**
     * Initializes the iterator to start at vertex 0.
     */
    void init() override;

    /**
     * Initializes the iterator to start at the given vertex.
     * @param v vertex in G
     */
    void init(uint64_t v);

    /**
     * Runs part of a DFS to check there are more vertices in the BCC.
     * @return true if there is another vertex in the BCC
     */
    bool more() override;

    /**
     * Gets the next vertex or edge of the BCC.
     * - Vertices in the form (u,INVALID)
     * - Edges in the form (u,v)
     * Order of output:
     *  1) vertex u
     *  2) edge (u,v)
     *  3) back edges to v (if any)
     *  4) vertex v
     * Note: The back edges to the start vertex of the BCC are output before the
     * start vertex itself. (start vertex: set in init())
     * @return next vertex or edge
     */
    std::pair<uint64_t, uint64_t> next() override;

    /**
     * Execute a given operation for each found pair (u,?).
     * (init() and start() before calling this method!)
     * @param f function to execute for each element
     */
    void forEach(
        std::function<void(std::pair<uint64_t, uint64_t>)> f) override {
        Iterator::forEach(f);
    }

 private:
    std::shared_ptr<EdgeMarker> e;
    UndirectedGraph const &g;
    uint64_t n;
    CompactArray color;
    StaticSpaceStorage parent;

    uint64_t startNode = 0;
    uint64_t node = 0, edge = 0;

    enum Action { OUTPUT_VERTEX, OUTPUT_BACK_EDGES };
    enum Status { HAVE_NEXT, WAITING, RETREAT };
    Action action;
    Status status;

    uint64_t latestNode = 0;
    bool gotRetreat = false;
    uint64_t tmp = 0;
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_BCCITERATOR_H_
