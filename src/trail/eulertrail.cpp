#include <sealib/iterator/eulertrail.h>
#include "simpletrailstructure.h"
#include "trailstructure.h"

using Sealib::EulerTrail;

template<class TrailStructureType>
EulerTrail<TrailStructureType>::iterator::iterator(
    const EulerTrail<TrailStructureType> &eulerTrail_, uint64_t nIndex_) :
    eulerTrail(eulerTrail_),
    nIndex(nIndex_),
    mIndex(static_cast<uint64_t>(eulerTrail.trailStarts.select(nIndex) - 1)),
    arc(mIndex > eulerTrail.trail.size() ?
        (uint64_t) -1 : eulerTrail.trail[mIndex].getStartingArc()),
    ending(false) {
}

template<class TrailStructureType>
std::tuple<uint64_t, bool> EulerTrail<TrailStructureType>::iterator::operator*() const {
    return std::make_tuple(mIndex, ending);
}

template<class TrailStructureType>
typename EulerTrail<TrailStructureType>::iterator
&EulerTrail<TrailStructureType>::iterator::operator++() {
    if (arc != (uint64_t) -1) {
        uint64_t uCross = eulerTrail.graph->getNode(mIndex).getAdj()[arc].second;
        mIndex = eulerTrail.graph->getNode(mIndex).getAdj()[arc].first;
        arc = eulerTrail.trail[mIndex].getMatched(uCross);
        if (arc == uCross) {
            arc = (uint64_t) -1;
            ending = true;
        } else {
            ending = false;
        }
    } else {
        ending = false;
        mIndex = static_cast<uint64_t>(eulerTrail.trailStarts.select(++nIndex)) - 1;
        arc = mIndex > eulerTrail.trail.size() ?
              (uint64_t) -1 : eulerTrail.trail[mIndex].getStartingArc();
    }
    return *this;
}

template<class TrailStructureType>
typename EulerTrail<TrailStructureType>::iterator
&EulerTrail<TrailStructureType>::iterator::operator++(int) {
    return ++(*this);
}

template<class TrailStructureType>
bool EulerTrail<TrailStructureType>::iterator::operator!=(const EulerTrail::iterator &rhs) const {
    return nIndex != rhs.nIndex;
}

template<class TrailStructureType>
typename EulerTrail<TrailStructureType>::iterator EulerTrail<TrailStructureType>::begin() const {
    return EulerTrail<TrailStructureType>::iterator(
        const_cast<EulerTrail<TrailStructureType> &>(*this), 1);
}
template<class TrailStructureType>
typename EulerTrail<TrailStructureType>::iterator EulerTrail<TrailStructureType>::end() const {
    return EulerTrail<TrailStructureType>::iterator(
        (const EulerTrail<TrailStructureType> &) *this,
        static_cast<uint64_t>(trailStarts.rank(trailStarts.size()) + 1));
}

template<class TrailStructureType>
EulerTrail<TrailStructureType>::EulerTrail(const std::shared_ptr<Sealib::UndirectedGraph> &graph_)
    : graph(graph_), trail(initializeTrail()), trailStarts(std::move(findTrailStarts())) {
}

template<class TrailStructureType>
uint64_t EulerTrail<TrailStructureType>::findStartingNode() {
    uint64_t order = graph->getOrder();

    for (uint64_t i = 0; i < order; i++) {
        bool isEven = trail.at(i).isEven();
        bool isBlack = trail.at(i).isBlack();
        if (!isEven && !isBlack) {  // odd
            return i;
        }
    }
    // no odd found, search for grey
    for (uint64_t i = 0; i < order; i++) {
        // first that has edges, it's possible to have a graph with no edges
        if (trail.at(i).isGrey()
            && !trail.at(i).isBlack()) {
            return i;
        }
    }
    // no odd found and no grey found, search for white
    for (uint64_t i = 0; i < order; i++) {
        // first that has edges, it's possible to have a graph with no edges
        if (!trail.at(i).isBlack()) {
            return i;
        }
    }
    return (uint64_t) -1;
}

template<class TrailStructureType>
std::vector<TrailStructureType>
EulerTrail<TrailStructureType>::initializeTrail() {
    std::vector<TrailStructureType> trail_;

    uint64_t order = graph->getOrder();
    trail_.reserve(order);
    for (uint64_t i = 0; i < graph->getOrder(); i++) {
        trail_.emplace_back(graph->getNode(i).getDegree());
    }
    trail_.shrink_to_fit();

    uint64_t u = findStartingNode();
    while (u != (uint64_t) -1) {  // loop the iteration while there is a non-black vertex
        auto kOld = (uint64_t) -1;
        if (trail_.at(u).isEven() && trail_.at(u).isGrey()) {  // remember aOld
            kOld = trail_.at(u).getLastClosed();
        }
        uint64_t kFirst = trail_.at(u).leave();

        uint64_t k = kFirst;
        uint64_t uMate;
        do {
            uMate = graph->getNode(u).getAdj()[k].second;
            u = graph->getNode(u).getAdj()[k].first;  // next node
            k = trail_.at(u).enter(uMate);
        } while (k != (uint64_t) -1);

        if (kOld != (uint64_t) -1) {
            uint64_t kLast = uMate;
            uint64_t kOldMatch = trail_.at(u).getMatched(kOld);
            if (kOldMatch != kOld) {  // has match
                trail_.at(u).marry(kOldMatch, kFirst);
                trail_.at(u).marry(kLast, kOld);
            } else {
                trail_.at(u).marry(kLast, kOld);
            }
        }
        // find next start node
        u = findStartingNode();
    }
    return trail_;
}

template<class TrailStructureType>
Sealib::Bitset<uint8_t>
EulerTrail<TrailStructureType>::findTrailStarts() {
    Sealib::Bitset<uint8_t> bs(graph->getOrder());
    for (uint64_t i = 0; i < graph->getOrder(); i++) {
        bool hasStarting = trail.at(i).hasStartingArc();
        if (hasStarting) {
            uint64_t arc = trail.at(i).getStartingArc();
            hasStarting = arc != (uint64_t) -1;
        }
        bs[i] = hasStarting;
    }
    return bs;
}

namespace Sealib {
template
class EulerTrail<SimpleTrailStructure>;
template
class EulerTrail<TrailStructure>;
}  // namespace Sealib
