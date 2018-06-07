//
// Created by jmeintrup on 13.04.18.
//


#include <cstdlib>
#include <ogdf/basic/GraphAttributes.h>
#include <include/sealib/simpletrailstructure.h>
#include <include/sealib/simpletrail.h>
#include "sealib/graphalgorithms.h"

Sealib::SimpleTrail Sealib::GraphAlgorithms::hierholzerSimple(Sealib::Graph *g) {
    static const std::tuple<unsigned int, unsigned int> nonArc = std::make_tuple((unsigned int) - 1, (unsigned int) - 1);
    Sealib::SimpleTrail trail;

    unsigned  int order = g->getOrder();
    std::vector<Sealib::SimpleTrailStructure*> ts;
    for (unsigned int i = 0; i < order; i ++) {
        unsigned  int degree = g->getNode(i).getDegree();
        ts.push_back(new Sealib::SimpleTrailStructure(degree));
    }

    // find first start node
    unsigned int u = (unsigned int) - 1;
    for (unsigned int i = 0; i < order; i ++) {
        if (g->getNode(i).getDegree() % 2 != 0) {  // odd
            u = i;
            break;
        }
    }
    if (u == (unsigned int) - 1) {  // no odd found
        for (unsigned int i = 0; i < order; i ++) {
            if (g->getNode(i).getDegree() != 0) {  // first that has edges, it's possible to have a graph with no edges
                u = i;
                break;
            }
        }
    }

    while (u != (unsigned int)  -1) {  // loop the iteration while there is a non-black vertex
        std::tuple<unsigned int, unsigned int> aOld = nonArc;
        if (ts[u]->isEven() && ts[u]->isGrey()) {  // remember aOld
            aOld = trail.getOutgoingFrom(u);
        }
        Sealib::SimpleTrail tempTrail;
        unsigned int k = ts[u]->leave();
        unsigned int uMate;
        do {
            unsigned int from = u;
            uMate = g->getNode(u).getAdj(k).crossIndex;
            u = g->getNode(u).getAdj(k).vertex;  // next node
            k = ts[u]->enter(uMate);
            tempTrail.addArc(std::make_tuple(from, u));
        } while (k != (unsigned int) - 1);

        if (aOld != nonArc) {
            unsigned long idx = trail.getFirstIndexOf(aOld);
            trail.insertSubTrail(tempTrail, idx);
        } else {
            trail.pushBackSubTrail(tempTrail);
        }

        // find next start node
        u = (unsigned int) - 1;

        for (unsigned int i = 0; i < order; i ++) {
            if (!ts[i]->isEven() && !ts[i]->isBlack()) {  // odd
                u = i;
                break;
            }
        }
        if (u == (unsigned int) - 1) {  // no odd found, search for grey
            for (unsigned int i = 0; i < order; i ++) {
                if (ts[i]->isGrey() && !ts[i]->isBlack()) {  // first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        if (u == (unsigned int) - 1) {  // no odd found and no grey found, search for white
            for (unsigned int i = 0; i < order; i ++) {
                if (!ts[i]->isBlack()) {  // first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        // no node left, should break now.
    }
    return trail;
}

Sealib::TrailStructure ** Sealib::GraphAlgorithms::hierholzer(Sealib::Graph *g) {
    unsigned  int order = g->getOrder();
    Sealib::TrailStructure **ts = static_cast<Sealib::TrailStructure **>(malloc((sizeof(Sealib::TrailStructure *) * order)));

    for (unsigned int i = 0; i < order; i ++) {
        unsigned  int degree = g->getNode(i).getDegree();
        ts[i] = new Sealib::TrailStructure(degree);
    }

    // find first start node
    unsigned int u = (unsigned int) - 1;
    for (unsigned int i = 0; i < order; i ++) {
        if (g->getNode(i).getDegree() % 2 != 0) {  // odd
            u = i;
            break;
        }
    }
    if (u == (unsigned int) - 1) {  // no odd found
        for (unsigned int i = 0; i < order; i ++) {
            if (g->getNode(i).getDegree() != 0) {  // first that has edges, it's possible to have a graph with no edges
                u = i;
                break;
            }
        }
    }

    while (u != (unsigned int)  -1) {  // loop the iteration while there is a non-black vertex
        unsigned int kOld = (unsigned int) - 1;
        if (ts[u]->isEven() && ts[u]->isGrey()) {  // remember aOld
            kOld = ts[u]->getLastClosed();
        }
        unsigned int kFirst = ts[u]->leave();

        unsigned int k = kFirst;
        unsigned int uMate;
        do {
            uMate = g->getNode(u).getAdj(k).crossIndex;
            u = g->getNode(u).getAdj(k).vertex;  // next node
            k = ts[u]->enter(uMate);
        } while (k != (unsigned int) - 1);

        if (kOld != (unsigned int) - 1) {
            unsigned int kLast = uMate;
            unsigned int kOldMatch = ts[u]->getMatched(kOld);
            if (kOldMatch != kOld) {  // has match
                ts[u]->marry(kOldMatch, kFirst);
                ts[u]->marry(kLast, kOld);
            } else {
                ts[u]->marry(kLast, kOld);
            }
        }

        // find next start node
        u = (unsigned int) - 1;

        for (unsigned int i = 0; i < order; i ++) {
            if (!ts[i]->isEven() && !ts[i]->isBlack()) {  // odd
                u = i;
                break;
            }
        }
        if (u == (unsigned int) - 1) {  // no odd found, search for grey
            for (unsigned int i = 0; i < order; i ++) {
                if (ts[i]->isGrey() && !ts[i]->isBlack()) {  // first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        if (u == (unsigned int) - 1) {  // no odd found and no grey found, search for white
            for (unsigned int i = 0; i < order; i ++) {
                if (!ts[i]->isBlack()) {  // first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        // no node left, should break now.
    }
    return ts;
}
