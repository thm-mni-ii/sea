//
// Created by jmeintrup on 31.05.18.
//

#include <sealib/simpletrailstructure.h>

unsigned int Sealib::SimpleTrailStructure::leave() {
    if(unusedEdges.empty()) {
        return (unsigned int) - 1;
    }
    unsigned int beg = unusedEdges.front();
    unusedEdges.pop_front();

    if(unusedEdges.empty()) {
        black = true;
    }
    grey = true;

    pairedEdges[beg] = (unsigned int) - 1;
    currentDegree--;
    return beg;
}

unsigned int Sealib::SimpleTrailStructure::enter(unsigned int idx) {
    if(unusedEdges.empty()) {
        return (unsigned int) - 1;
    }
    unsigned long size = unusedEdges.size();
    unusedEdges.remove(idx);
    if(size == unusedEdges.size()) {
        return (unsigned int) - 1;  // couldn't remove that idx
    }
    currentDegree--;

    if(unusedEdges.empty()) {
        black = true;
        pairedEdges[idx] = (unsigned int) - 1;
        return (unsigned int) - 1;
    }
    grey = true;

    unsigned int next = leave();
    if(next != (unsigned int) - 1) {
        pairedEdges[next] = idx;
        pairedEdges[idx] = next;
    }
    return next;
}



bool Sealib::SimpleTrailStructure::isBlack() {
    return black;
}

bool Sealib::SimpleTrailStructure::isWhite() {
    return !isGrey();
}

bool Sealib::SimpleTrailStructure::isGrey() {
    return grey;
}

bool Sealib::SimpleTrailStructure::isEven() {
    return currentDegree % 2 == 0;
}

Sealib::SimpleTrailStructure::SimpleTrailStructure(unsigned int degree) :
        currentDegree(degree),
        black(degree == 0),
        grey(false){
    for(unsigned int i = 0; i < degree; i++) {
        unusedEdges.push_back(i);
    }
}

unsigned int Sealib::SimpleTrailStructure::getMatched(unsigned int i) {
    if(pairedEdges.count(i)) {
        return pairedEdges[i];
    } else {
        return (unsigned int) - 1;
    }
}
