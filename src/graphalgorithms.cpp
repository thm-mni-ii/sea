//
// Created by jmeintrup on 13.04.18.
//

#include <sealib/trailstructure.h>
#include <cstdlib>
#include "sealib/graphalgorithms.h"

TrailStructure ** GraphAlgorithms::hierholzer(Graph *g) {

    unsigned  int order = g->getOrder();
    TrailStructure **ts = static_cast<TrailStructure **>(malloc((sizeof(TrailStructure *) * order)));

    for(unsigned int i = 0; i < order; i ++) {
        unsigned  int degree = g->getNode(i)->getDegree();
        ts[i] = new TrailStructure(degree);
    }

    //find first start node
    unsigned int u = (unsigned int) - 1;
    for(unsigned int i = 0; i < order; i ++) {
        if(g->getNode(i)->getDegree() % 2 != 0) { //odd
            u = i;
            break;
        }
    }
    if(u == (unsigned int) - 1) { //no odd found
        for(unsigned int i = 0; i < order; i ++) {
            if(g->getNode(i)->getDegree() != 0) { //first that has edges, it's possible to have a graph with no edges
                u = i;
                break;
            }
        }
    }

    while(u != (unsigned int)  -1) { //loop the iteration while there is a non-black vertex
        unsigned int kOld = (unsigned int) - 1;
        if(ts[u]->isEven() && ts[u]->isGrey()) { //remember aOld
            kOld = ts[u]->getLastClosed();
        }
        unsigned int kFirst = ts[u]->leave();
        //unsigned int kFirstMate = g.getNode(u)->getAdj()[kFirst].crossIndex;

        unsigned int k = kFirst;
        unsigned int uMate;
        do {
            uMate = g->getNode(u)->getAdj()[k].crossIndex;
            u = g->getNode(u)->getAdj()[k].vertex; //next node
            k = ts[u]->enter(uMate);
        } while(k != (unsigned int) - 1);

        if(kOld != (unsigned int) - 1) {
            unsigned int kLast = uMate;

            unsigned int kOldMatch = ts[u]->getMatched(kOld);
            if(kOldMatch != kOld) { //has match
                ts[u]->marry(kOldMatch, kFirst);
                ts[u]->marry(kLast, kOld);
            } else {
                ts[u]->marry(kLast, kOld);
            }
        }

        //find next start node
        u = (unsigned int) - 1;

        for(unsigned int i = 0; i < order; i ++) {
            if(!ts[i]->isEven() && !ts[i]->isBlack()) { //odd
                u = i;
                break;
            }
        }
        if(u == (unsigned int) - 1) { //no odd found, search for grey
            for(unsigned int i = 0; i < order; i ++) {
                if(ts[i]->isGrey() && !ts[i]->isBlack()) { //first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        if(u == (unsigned int) - 1) { //no odd found and no grey found, search for white
            for(unsigned int i = 0; i < order; i ++) {
                if(!ts[i]->isBlack()) { //first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        //no node left, should break now.
    }

    return ts;
}

std::string GraphAlgorithms::stringFromTrail(Graph *g, TrailStructure **trail, unsigned int order) {

}
