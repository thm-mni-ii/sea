#ifndef SEALIB_ITERATOR_BFS_H_
#define SEALIB_ITERATOR_BFS_H_

#include <utility>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/graph/graph.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {

const uint BFS_WHITE = 0, BFS_GRAY1 = 1, BFS_GRAY2 = 2, BFS_BLACK = 3;

const Consumer BFS_NOP_PROCESS = [](uint) {};
const BiConsumer BFS_NOP_EXPLORE = [](uint, uint) {};

/**
 * Run a breadth-first search over a given graph, while executing the two
 * given user functions.
 * EFFICIENCY: O(n+m) time, O(n) bits
 *
 * The BFS is implemented as an iterator. To start the iterator, call init().
 * To see if there are more results in this component, call more().
 * To get the next result, call next().
 * To move the search to a possible next component, call nextComponent().
 */
class BFS : Iterator<std::pair<uint, uint>> {
 public:
    /**
    * Create a new BFS iterator.
    * @param g the graph to run the BFS over
    * @param preprocess to be executed before processing a node u
    * @param preexplore to be executed before exploring an edge (u,v)
    */
    BFS(Graph const *g, Consumer pp, BiConsumer pe);

    BFS(Graph const *g, CompactArray color, Consumer pp, BiConsumer pe);

    /**
     * Initialize or reset the BFS to the beginning.
     */
    void init() override;

    /**
     * Check if there is a component that has not been evaluated. If so, the BFS
     * will continue there.
     * @return true if a component was found, false if all nodes have been
     * evaluated
     */
    bool nextComponent();

    /**
     * @return true if this component has unevaluated nodes, false if the
     * component is all black
     */
    bool more() override;

    /**
     * Get the next node from the current component.
     * @return a tuple (u,d) where d is the distance of u to the component's
     * starting node
     * @throws std::logic_error if no next node is available
     */
    std::pair<uint, uint> next() override;

    /**
     * Execute a given operation for each found pair (u,dist).
     * (init() before calling this method!)
     * @param f function to execute for each element
     */
    void forEach(std::function<void(std::pair<uint, uint>)> f) override;

 private:
    Graph const *g;
    uint n;
    CompactArray color;
    uint u, dist;
    uint32_t innerGray, outerGray;
    ChoiceDictionary isInner, isOuter;
    Consumer preprocess;
    BiConsumer preexplore;

    bool hasGrayNode();
    uint getGrayNode();
};

}  // namespace Sealib
#endif  // SEALIB_ITERATOR_BFS_H_
