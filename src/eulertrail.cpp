#include <sealib/eulertrail.h>
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
EulerTrail<TrailStructureType>::iterator::iterator(
    const EulerTrail<TrailStructureType> &eulerTrail_, unsigned int nIndex_) :
    eulerTrail(eulerTrail_),
    nIndex(nIndex_),
    mIndex(eulerTrail.trailStarts.select(nIndex) - 1),
    arc(mIndex > eulerTrail.trail.size() ?
        (unsigned int) -1 : eulerTrail.trail[mIndex].getStartingArc()),
    ending(false) {
}

template<class TrailStructureType>
std::tuple<unsigned long, bool> EulerTrail<TrailStructureType>::iterator::operator*() const {
    return std::make_tuple(mIndex, ending);
}

template<class TrailStructureType>
typename EulerTrail<TrailStructureType>::iterator
&EulerTrail<TrailStructureType>::iterator::operator++() {
    if (arc != (unsigned int) -1) {
        unsigned int uCross = eulerTrail.graph->getNode(mIndex).getAdj()[arc].crossIndex;
        mIndex = eulerTrail.graph->getNode(mIndex).getAdj()[arc].vertex;
        arc = eulerTrail.trail[mIndex].getMatched(uCross);
        if (arc == uCross) {
            arc = (unsigned int) -1;
            ending = true;
        } else {
            ending = false;
        }
    } else {
        ending = false;
        mIndex = eulerTrail.trailStarts.select(++nIndex) - 1;
        arc = mIndex > eulerTrail.trail.size() ?
              (unsigned int) -1 : eulerTrail.trail[mIndex].getStartingArc();
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
        (EulerTrail<TrailStructureType> &) *this, trailStarts.rank(trailStarts.size()) + 1);
}

template<class TrailStructureType>
EulerTrail<TrailStructureType>::EulerTrail(const graphptr_t &graph_)
    : graph(graph_), trail(initializeTrail()), trailStarts(findTrailStarts()) {
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
typename EulerTrail<TrailStructureType>::array_t
EulerTrail<TrailStructureType>::initializeTrail() {
    EulerTrail<TrailStructureType>::array_t trail_;

    unsigned int order = graph->getOrder();
    trail_.reserve(order);
    for (unsigned int i = 0; i < graph->getOrder(); i++) {
        trail_.emplace_back(graph->getNode(i).getDegree());
    }
    trail_.shrink_to_fit();

    unsigned int u = findStartingNode();
    while (u != (unsigned int) -1) {  // loop the iteration while there is a non-black vertex
        auto kOld = (unsigned int) -1;
        if (trail_.at(u).isEven() && trail_.at(u).isGrey()) {  // remember aOld
            kOld = trail_.at(u).getLastClosed();
        }
        unsigned int kFirst = trail_.at(u).leave();

        unsigned int k = kFirst;
        unsigned int uMate;
        do {
            uMate = graph->getNode(u).getAdj()[k].crossIndex;
            u = graph->getNode(u).getAdj()[k].vertex;  // next node
            k = trail_.at(u).enter(uMate);
        } while (k != (unsigned int) -1);

        if (kOld != (unsigned int) -1) {
            unsigned int kLast = uMate;
            unsigned int kOldMatch = trail_.at(u).getMatched(kOld);
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
typename EulerTrail<TrailStructureType>::bitset_t
EulerTrail<TrailStructureType>::findTrailStarts() {
    EulerTrail<TrailStructureType>::bitset_t bs(graph->getOrder());
    for (unsigned int i = 0; i < graph->getOrder(); i++) {
        bool hasStarting = trail.at(i).hasStartingArc();
        if (hasStarting) {
            unsigned int arc = trail.at(i).getStartingArc();
            hasStarting = arc != (unsigned int) -1;
        }
        bs[i] = hasStarting;
    }
    return bs;
}
