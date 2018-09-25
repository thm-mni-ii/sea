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
    static std::unique_ptr<std::vector<TrailStructureType>> hierholzer(const BasicGraph &graph) {
        unsigned int order = graph.getOrder();
        std::unique_ptr<std::vector<TrailStructureType>> trail(
            new std::vector<TrailStructureType>());

        for (unsigned int i = 0; i < order; i++) {
            trail->emplace_back(graph.getNode(i).getDegree());
        }

        auto u = (unsigned int) -1;
        for (unsigned int i = 0; i < order; i++) {
            if (graph.getNode(i).getDegree() % 2 != 0) {  // odd
                u = i;
                break;
            }
        }
        if (u == (unsigned int) -1) {  // no odd found
            for (unsigned int i = 0; i < order; i++) {
                if (graph.getNode(i).getDegree()
                    != 0) {  // first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }

        while (u != (unsigned int) -1) {  // loop the iteration while there is a non-black vertex
            auto kOld = (unsigned int) -1;
            if (trail->at(u).isEven() && trail->at(u).isGrey()) {  // remember aOld
                kOld = trail->at(u).getLastClosed();
            }
            unsigned int kFirst = trail->at(u).leave();

            unsigned int k = kFirst;
            unsigned int uMate;
            do {
                uMate = graph.getNode(u).getAdj()[k].crossIndex;
                u = graph.getNode(u).getAdj()[k].vertex;  // next node
                k = trail->at(u).enter(uMate);
            } while (k != (unsigned int) -1);

            if (kOld != (unsigned int) -1) {
                unsigned int kLast = uMate;
                unsigned int kOldMatch = trail->at(u).getMatched(kOld);
                if (kOldMatch != kOld) {  // has match
                    trail->at(u).marry(kOldMatch, kFirst);
                    trail->at(u).marry(kLast, kOld);
                } else {
                    trail->at(u).marry(kLast, kOld);
                }
            }

            // find next start node
            u = (unsigned int) -1;

            for (unsigned int i = 0; i < order; i++) {
                if (!trail->at(i).isEven() && !trail->at(i).isBlack()) {  // odd
                    u = i;
                    break;
                }
            }
            if (u == (unsigned int) -1) {  // no odd found, search for grey
                for (unsigned int i = 0; i < order; i++) {
                    // first that has edges, it's possible to have a graph with no edges
                    if (trail->at(i).isGrey()
                        && !trail->at(i).isBlack()) {
                        u = i;
                        break;
                    }
                }
            }
            if (u == (unsigned int) -1) {  // no odd found and no grey found, search for white
                for (unsigned int i = 0; i < order; i++) {
                    // first that has edges, it's possible to have a graph with no edges
                    if (!trail->at(i).isBlack()) {
                        u = i;
                        break;
                    }
                }
            }
            // no node left, should break now.
        }
        return trail;
    }
};
}  // namespace Sealib



#endif  // SEALIB_GRAPHALGORITHM_H_
