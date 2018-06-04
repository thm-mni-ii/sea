//
// Created by jmeintrup on 31.05.18.
//

#ifndef SEA_SIMPLETRAILSTRUCTURE_H
#define SEA_SIMPLETRAILSTRUCTURE_H

#include "doublelinkedlist.h"
#include <map>
#include <list>

namespace Sealib {
    class SimpleTrailStructure {
    private:
        std::map<unsigned int, unsigned int> pairedEdges;
        std::list<unsigned int> unusedEdges;
        unsigned int currentDegree;
        bool black;
        bool white;
        bool grey;
    public:
        SimpleTrailStructure(unsigned int degree_);
        unsigned int leave();
        unsigned int enter(unsigned int e);
        bool isBlack();
        bool isWhite();
        bool isGrey();
        bool isEven();
        unsigned int getMatched(unsigned int i);
    };
}
#endif //SEA_SIMPLETRAILSTRUCTURE_H
