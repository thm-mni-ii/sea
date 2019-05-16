#ifndef SEALIB_ITERATOR_SCCITERATOR_H_
#define SEALIB_ITERATOR_SCCITERATOR_H_
#include "sealib/iterator/reversedfs.h"

namespace Sealib {
/**
 * Iterator over the strongly connected components of a directed graph.
 * After calling init(), use more() and next() to run DFS procedures over the
 * SCCs of the input graph.
 * @author Simon Heuser
 *
 * EFFICIENCY: O(n+m) time, O(n log(log(n))) bits
 */
class SCCIterator : public Iterator<uint64_t> {
 public:
    /**
     * Creates a new SCC iterator.
     * You may supply user calls that are executed during the DFS of a found SCC
     * (see `next()`).
     * @param g the input graph
     * @param preprocess action to execute before processing a vertex u
     * @param preexplore action to execute before exploring an edge (u,k)
     * @param postexplore action to execute after exploring an edge (u,k)
     * @param postprocess action to execute after processing a vertex u
     */
    SCCIterator(DirectedGraph const &g, Consumer preprocess = DFS_NOP_PROCESS,
                BiConsumer preexplore = DFS_NOP_EXPLORE,
                BiConsumer postexplore = DFS_NOP_EXPLORE,
                Consumer postprocess = DFS_NOP_PROCESS);

    /**
     * Initializes the iterator.
     */
    void init() override;

    /**
     * Checks if there are more undiscovered SCCs in the input graphs.
     * @return true if another SCC was found
     */
    bool more() override;

    /**
     * Runs a DFS procedure over the found SCC. The user calls supplied to the
     * constructor are executed as specified.
     * @return the root vertex of the (now finished) SCC
     * 
     * Example (output all vertices in the first SCC):
     *   SCCIterator s(g,[](uint u){ printf("found vertex %lu\n",u); });
     *   s.init();
     *   s.more();
     *   s.next();
     */
    uint64_t next() override;

 private:
    DirectedGraph const &g;
    uint64_t n, u0;
    DirectedGraph t;
    ReverseDFS d;
    CompactArray c;
    ExtendedSegmentStack s;
    Consumer preprocess;
    BiConsumer preexplore;
    BiConsumer postexplore;
    Consumer postprocess;

    uint64_t nextFinished();
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_SCCITERATOR_H_
