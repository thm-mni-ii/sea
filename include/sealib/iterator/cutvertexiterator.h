#ifndef SEALIB_ITERATOR_CUTVERTEXITERATOR_H_
#define SEALIB_ITERATOR_CUTVERTEXITERATOR_H_
#include <memory>
#include "sealib/_types.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/choicedictionaryiterator.h"
#include "sealib/iterator/dfs.h"
#include "sealib/iterator/edgemarker.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {
/**
 * This iterator can output all the cut vertices for an undirected graph G in
 * order. There is also the possibility to check if a specific vertex is a cut
 * vertex.
 */
class CutVertexIterator : Iterator<uint>, DFS {
 public:
    /**
     * Create a new cut-vertex iterator for an undirected graph G.
     * @param g the undirected graph G=(V,E)
     */
    explicit CutVertexIterator(UndirectedGraph *g);

    /**
     * Create a new cut-vertex iterator from a given edge marker (allows
     * recycling).
     * @param e shared pointer to an EdgeMarker
     */
    explicit CutVertexIterator(std::shared_ptr<EdgeMarker> e);

    /**
     * Initialize the iterator: first identify all tree and back edges, then
     * mark tree edges, finally enter cut vertices into a local choice
     * dictionary.
     */
    void init() override;

    /**
     * @return true if there are more cut vertices that have not been output
     */
    bool more() override;

    /**
     * @return the next cut vertex of the graph
     */
    uint next() override;

    /**
     * Check if a given vertex is a cut vertex of the graph.
     * (The iterator must be initialized first.)
     * @param u vertex to check status for
     * @return true if the vertex is a cut vertex (i.e. u has at least one
     * "outgoing" edge that is not full marked)
     */
    bool isCutVertex(uint u);

 private:
    std::shared_ptr<EdgeMarker> e;
    UndirectedGraph *g;
    uint n;
    ChoiceDictionary cc;
    ChoiceDictionary cut;
    ChoiceDictionaryIterator cutI;

    void markParents(uint w, uint u, StaticSpaceStorage *parent);
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_CUTVERTEXITERATOR_H_
