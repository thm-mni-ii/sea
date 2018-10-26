#include <src/naivetrailstructure.h>

unsigned int Sealib::NaiveTrailStructure::leave() {
    if (unusedEdges.empty()) {
        return (unsigned int) -1;
    }
    unsigned int beg = unusedEdges.front();
    unusedEdges.pop_front();

    if (unusedEdges.empty()) {
        black = true;
    }
    grey = true;

    pairedEdges[beg] = (unsigned int) -1;
    currentDegree--;
    return beg;
}

unsigned int Sealib::NaiveTrailStructure::enter(unsigned int idx) {
    if (unusedEdges.empty()) {
        return (unsigned int) -1;
    }
    unsigned long size = unusedEdges.size();
    unusedEdges.remove(idx);
    if (size == unusedEdges.size()) {
        return (unsigned int) -1;  // couldn't remove that idx
    }
    currentDegree--;

    if (unusedEdges.empty()) {
        black = true;
        pairedEdges[idx] = (unsigned int) -1;
        return (unsigned int) -1;
    }
    grey = true;

    unsigned int next = leave();
    if (next != (unsigned int) -1) {
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

Sealib::NaiveTrailStructure::NaiveTrailStructure(unsigned int degree) :
    currentDegree(degree),
    black(degree == 0),
    grey(false) {
    for (unsigned int i = 0; i < degree; i++) {
        unusedEdges.push_back(i);
    }
}

unsigned int Sealib::NaiveTrailStructure::getMatched(unsigned int i) {
    if (pairedEdges.count(i)) {
        return pairedEdges[i];
    } else {
        return (unsigned int) -1;
    }
}
