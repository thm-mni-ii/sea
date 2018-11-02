#ifndef SRC_NAIVEEULERTRAIL_H_
#define SRC_NAIVEEULERTRAIL_H_

#include <sealib/graph/basicgraph.h>
#include <src/trail/simpletrail.h>
#include <ostream>
#include <vector>
#include <list>
#include <memory>

using Sealib::BasicGraph;
using std::shared_ptr;
using std::vector;
using std::list;

namespace Sealib {
/**
 * Space efficient Euler Trail class. Initialized with an undirected graph object G_0,
 * creates a set of euler partitions for G_0 during construction.
 * Uses O(n+m) time for the construction and occupies O(n) space after construction.
 * @tparam TrailStructureType
 */
class NaiveEulerTrail {
 private:
    std::list<Sealib::SimpleTrail> trails;
    shared_ptr<BasicGraph> graph;

 public:
    /**
     *
     * @param graph - undirected graph object for which the euler trails should be created
     */
    explicit NaiveEulerTrail(const shared_ptr<BasicGraph> &graph);
};
}  // namespace Sealib
#endif  //  SRC_NAIVEEULERTRAIL_H_
