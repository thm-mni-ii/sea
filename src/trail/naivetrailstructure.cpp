#include "./naivetrailstructure.h"

uint32_t Sealib::NaiveTrailStructure::leave() {
    if (unusedEdges.empty()) {
        return (uint32_t) -1;
    }
    uint32_t beg = unusedEdges.front();
    unusedEdges.pop_front();

    if (unusedEdges.empty()) {
        black = true;
    }
    grey = true;

    pairedEdges[beg] = (uint32_t) -1;
    currentDegree--;
    return beg;
}

uint32_t Sealib::NaiveTrailStructure::enter(uint32_t idx) {
    if (unusedEdges.empty()) {
        return (uint32_t) -1;
    }
    uint64_t size = unusedEdges.size();
    unusedEdges.remove(idx);
    if (size == unusedEdges.size()) {
        return (uint32_t) -1;  // couldn't remove that idx
    }
    currentDegree--;

    if (unusedEdges.empty()) {
        black = true;
        pairedEdges[idx] = (uint32_t) -1;
        return (uint32_t) -1;
    }
    grey = true;

    uint32_t next = leave();
    if (next != (uint32_t) -1) {
        pairedEdges[next] = idx;
        pairedEdges[idx] = next;
    }
    return next;
}

bool Sealib::NaiveTrailStructure::isBlack() {
    return black;
}

bool Sealib::NaiveTrailStructure::isWhite() {
    return !isGrey();
}

bool Sealib::NaiveTrailStructure::isGrey() {
    return grey;
}

bool Sealib::NaiveTrailStructure::isEven() {
    return currentDegree % 2 == 0;
}

Sealib::NaiveTrailStructure::NaiveTrailStructure(uint32_t degree) :
    currentDegree(degree),
    black(degree == 0),
    grey(false) {
    for (uint32_t i = 0; i < degree; i++) {
        unusedEdges.push_back(i);
    }
}

uint32_t Sealib::NaiveTrailStructure::getMatched(uint32_t i) {
    if (pairedEdges.count(i)) {
        return pairedEdges[i];
    } else {
        return (uint32_t) -1;
    }
}
