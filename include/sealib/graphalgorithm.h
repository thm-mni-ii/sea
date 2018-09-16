#ifndef SEALIB_GRAPHALGORITHM_H_
#define SEALIB_GRAPHALGORITHM_H_

#include <sealib/simpletrailstructure.h>
#include <sealib/naivetrail.h>
#include <sealib/basicgraph.h>
#include <sealib/trailstructure.h>
#include <memory>

namespace Sealib {
class GraphAlgorithm {
 public:
    static std::unique_ptr<std::vector<SimpleTrailStructure>>
    hierholzerSimple(BasicGraph &graph);

    static std::unique_ptr<std::vector<TrailStructure>>
    hierholzer(BasicGraph &graph);

    static std::unique_ptr<NaiveTrail>
    hierholzerNaive(BasicGraph &graph);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPHALGORITHM_H_
