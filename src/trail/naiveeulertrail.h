#ifndef SRC_TRAIL_NAIVEEULERTRAIL_H_
#define SRC_TRAIL_NAIVEEULERTRAIL_H_

#include <sealib/graph/undirectedgraph.h>
#include "./simpletrail.h"
#include <ostream>
#include <vector>
#include <list>
#include <memory>

namespace Sealib {
/**
 * Space efficient Euler Trail class. Initialized with an undirected graph object G_0,
 * creates a set of euler partitions for G_0 during construction.
 * Uses O(n+m) time for the construction and occupies O(n) space after construction.
 */
class NaiveEulerTrail {
 private:
    std::list<Sealib::SimpleTrail> trails;
    std::shared_ptr<UndirectedGraph> graph;

 public:
    /**
     *
     * @param graph - undirected graph object for which the euler trails should be created
     */
    explicit NaiveEulerTrail(const std::shared_ptr<UndirectedGraph> &graph);
};
}  // namespace Sealib
#endif  //  SRC_TRAIL_NAIVEEULERTRAIL_H_
