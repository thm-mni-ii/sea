#ifndef SEALIB_BFS_H_
#define SEALIB_BFS_H_

#include <functional>
#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/graph.h"
#include "sealib/iterator.h"

#define BFS_WHITE 0
#define BFS_GRAY1 1
#define BFS_GRAY2 2
#define BFS_BLACK 3

#define BFS_NOP_PROCESS [](uint) {}        // NOLINT
#define BFS_NOP_EXPLORE [](uint, uint) {}  // NOLINT

namespace Sealib {
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
class BFS : Iterator<Pair> {
 public:
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
  Pair next() override;

  /**
   * Create a new BFS iterator.
  * @param g the graph to run the BFS over
  * @param preprocess to be executed before processing a node u
  * @param preexplore to be executed before exploring an edge (u,v)
  */
  BFS(Graph *graph, UserFunc1 preprocess, UserFunc2 preexplore);
  BFS(Graph *g, CompactArray *c, UserFunc1 preprocess, UserFunc2 preexplore);

  ~BFS();

 private:
  Graph *g;
  uint n;
  CompactArray *color;
  bool externalCA = false;
  uint u, dist;
  unsigned innerGray, outerGray;
  UserFunc1 preprocess;
  UserFunc2 preexplore;

  bool hasGrayNode();
  uint getGrayNode();
};

}  // namespace Sealib
#endif  // SEALIB_BFS_H_
