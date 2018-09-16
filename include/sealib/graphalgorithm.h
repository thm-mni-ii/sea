#ifndef SEALIB_GRAPHALGORITHM_H_
#define SEALIB_GRAPHALGORITHM_H_

#include <sealib/simpletrailstructure.h>
#include <sealib/naivetrail.h>
#include <sealib/basicgraph.h>
#include <sealib/trailstructure.h>
#include <memory>
#include <vector>

namespace Sealib {
class GraphAlgorithm {
 public:
    template<typename TrailStructureType>
    std::unique_ptr<std::vector<TrailStructureType>> hierholzer(const BasicGraph &graph);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPHALGORITHM_H_
