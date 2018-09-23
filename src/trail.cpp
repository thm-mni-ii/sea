#include <sealib/trail.h>
#include <sealib/simpletrailstructure.h>
#include <sealib/trailstructure.h>

using Sealib::EulerTrail;

namespace Sealib {
template
class EulerTrail<SimpleTrailStructure>;
template
class EulerTrail<TrailStructure>;
}

template<class TrailStructureType>
EulerTrail<TrailStructureType>::EulerTrail(const graphptr_t &graph)
    : graph(graph), trail(initializeTrail()), trailStarts(findTrailStarts()) {
}

template<class TrailStructureType>
unsigned int EulerTrail<TrailStructureType>::findStartingNode() {
    unsigned int order = graph->getOrder();

    for (unsigned int i = 0; i < order; i++) {
        bool isEven = trail.at(i).isEven();
        bool isBlack = trail.at(i).isBlack();
        if (!isEven && !isBlack) {  // odd
            return i;
        }
    }
    // no odd found, search for grey
    for (unsigned int i = 0; i < order; i++) {
        // first that has edges, it's possible to have a graph with no edges
        if (trail.at(i).isGrey()
            && !trail.at(i).isBlack()) {
            return i;
        }
    }
    // no odd found and no grey found, search for white
    for (unsigned int i = 0; i < order; i++) {
        // first that has edges, it's possible to have a graph with no edges
        if (!trail.at(i).isBlack()) {
            return i;
        }
    }
    return (unsigned int) -1;
}

template<class TrailStructureType>
vector<TrailStructureType> EulerTrail<TrailStructureType>::initializeTrail() {
    vector<TrailStructureType> trail;

    unsigned int order = graph->getOrder();
    trail.reserve(order);
    for (unsigned int i = 0; i < graph->getOrder(); i++) {
        trail.emplace_back(graph->getNode(i).getDegree());
    }
    trail.shrink_to_fit();

    unsigned int u = findStartingNode();
    while (u != (unsigned int) -1) {  // loop the iteration while there is a non-black vertex
        auto kOld = (unsigned int) -1;
        if (trail.at(u).isEven() && trail.at(u).isGrey()) {  // remember aOld
            kOld = trail.at(u).getLastClosed();
        }
        unsigned int kFirst = trail.at(u).leave();

        unsigned int k = kFirst;
        unsigned int uMate;
        do {
            uMate = graph->getNode(u).getAdj()[k].crossIndex;
            u = graph->getNode(u).getAdj()[k].vertex;  // next node
            k = trail.at(u).enter(uMate);
        } while (k != (unsigned int) -1);

        if (kOld != (unsigned int) -1) {
            unsigned int kLast = uMate;
            unsigned int kOldMatch = trail.at(u).getMatched(kOld);
            if (kOldMatch != kOld) {  // has match
                trail.at(u).marry(kOldMatch, kFirst);
                trail.at(u).marry(kLast, kOld);
            } else {
                trail.at(u).marry(kLast, kOld);
            }
        }
        // find next start node
        u = findStartingNode();
    }
    return trail;
}

template<class TrailStructureType>
bitset_t EulerTrail<TrailStructureType>::findTrailStarts() {
    bitset_t bs(graph->getOrder());
    for (unsigned int i = 0; i < graph->getOrder(); i++) {
        bool hasStarting = trail.at(i).hasStartingArc();
        bs[i] = hasStarting;
    }
    return bs;
}
