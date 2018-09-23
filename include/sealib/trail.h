#ifndef SEALIB_TRAIL_H_
#define SEALIB_TRAIL_H_

#include <sealib/rankselect.h>
#include <sealib/basicgraph.h>
#include <sealib/simpletrailstructure.h>
#include <sealib/trailstructure.h>
#include <ostream>
#include <vector>

using rankselect_t = Sealib::RankSelect;
using std::vector;
using graph_t = Sealib::BasicGraph;
using graphptr_t = std::shared_ptr<graph_t>;
using bitset_t = Sealib::Bitset<unsigned char>;

namespace Sealib {
template<class TrailStructureType>
class EulerTrail {
 private:
    graphptr_t graph;
    vector<TrailStructureType> trail;
    rankselect_t trailStarts;

    unsigned int findStartingNode();
    bitset_t findTrailStarts();

 public:
    explicit EulerTrail(const graphptr_t &graph);

    vector<TrailStructureType> initializeTrail();

    friend std::ostream &operator<<(std::ostream &os,
                                    const EulerTrail<TrailStructureType> &eulerTrail) {
        unsigned long maxRank = eulerTrail.trailStarts.rank(eulerTrail.trailStarts.size());
        for (unsigned int i = 1; i <= maxRank; i++) {
            unsigned int v0 = static_cast<unsigned int>(eulerTrail.trailStarts.select(i)) - 1;
            unsigned int arc = eulerTrail.trail[v0].getStartingArc();
            if (arc != (unsigned int) -1) {
                os << "T" << i << ": " << v0 << " -> ";
                unsigned int u = v0;
                while (arc != (unsigned int) -1) {
                    unsigned int uCross = eulerTrail.graph->getNode(u).getAdj()[arc].crossIndex;
                    u = eulerTrail.graph->getNode(u).getAdj()[arc].vertex;

                    os << u;
                    arc = eulerTrail.trail[u].getMatched(uCross);
                    if (arc == uCross) {
                        arc = (unsigned int) -1;
                        os << std::endl;
                    } else {
                        os << " -> ";
                    }
                }
            }
        }
        return os;
    }
};
}  // namespace Sealib
#endif  // SEALIB_TRAIL_H_
