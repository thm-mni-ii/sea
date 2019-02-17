#include "./naivetrailstructure.h"

uint64_t Sealib::NaiveTrailStructure::leave() {
    if (unusedEdges.empty()) {
        return INVALID;
    }
    uint64_t beg = unusedEdges.front();
    unusedEdges.pop_front();

    if (unusedEdges.empty()) {
        black = true;
    }
    grey = true;

    pairedEdges[beg] = INVALID;
    currentDegree--;
    return beg;
}

uint64_t Sealib::NaiveTrailStructure::enter(uint64_t idx) {
    if (unusedEdges.empty()) {
        return INVALID;
    }
    uint64_t size = unusedEdges.size();
    unusedEdges.remove(idx);
    if (size == unusedEdges.size()) {
        return INVALID;  // couldn't remove that idx
    }
    currentDegree--;

    if (unusedEdges.empty()) {
        black = true;
        pairedEdges[idx] = INVALID;
        return INVALID;
    }
    grey = true;

    uint64_t next = leave();
    if (next != INVALID) {
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

Sealib::NaiveTrailStructure::NaiveTrailStructure(uint64_t degree) :
    currentDegree(degree),
    black(degree == 0),
    grey(false) {
    for (uint64_t i = 0; i < degree; i++) {
        unusedEdges.push_back(i);
    }
}

uint64_t Sealib::NaiveTrailStructure::getMatched(uint64_t i) {
    if (pairedEdges.count(i)) {
        return pairedEdges[i];
    } else {
        return INVALID;
    }
}
