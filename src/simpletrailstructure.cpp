#include <sealib/simpletrailstructure.h>
#include <cstdlib>
#include <iostream>

Sealib::SimpleTrailStructure::SimpleTrailStructure(unsigned int _degree) :
    degree(_degree),
    nextUnused(1),
    lastClosed((unsigned int) -1),
    inAndOut(degree),
    matched(degree),
    flags(3),
    unused(degree * 3) {
    nextUnused = 1;
    lastClosed = (unsigned int) -1;

    if (degree % 2 != 0) flags[2] = 1;  // set it to grey if uneven
    if (degree == 0) flags[1] = 1;  // node with no edges is possible, set black

    for (unsigned int &i : married) {
        i = (unsigned int) -1;
    }
    // [1][0][1] [1][1][1] [1][2][1] ... [1][degree-1][1]
    for (unsigned int i = 0; i < degree; i++) {
        unused[i * 3] = 1;
        unused[i * 3 + 1] = i;
        unused[i * 3 + 2] = 1;
    }
}

unsigned int Sealib::SimpleTrailStructure::getNextUnused() {
    if (flags[1]) {  // black node
        return (unsigned int) -1;
    }

    if (!flags[0]) {
        // cout << "Set to grey\n";
        flags[0] = 1;
    }
    // set to grey
    unsigned int prevLink = unused[nextUnused - 1];
    unsigned int nextLink = unused[nextUnused + 1];

    unsigned int temp;

    if (prevLink * 3 > nextUnused) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + nextUnused;
    } else {
        temp = nextUnused - prevLink * 3;
    }
    if (temp == nextUnused) {  // no other element, this is last
        unsigned int retVal = nextUnused;
        nextUnused = 0;
        flags[1] = !flags[1];
        return retVal;
    }
unused[temp + 1] += nextLink;

    if (nextLink * 3 + nextUnused > degree * 3) {  // circle around
        temp = (nextLink * 3) - (degree * 3) + nextUnused;
    } else {
        temp = nextUnused + nextLink * 3;
    }
    unused[temp - 1] += prevLink;
    unsigned int retVal = nextUnused;
    nextUnused = temp;
    // taking an arc flips the parity
    flags[2] = !flags[2];

    return retVal;
}

unsigned int Sealib::SimpleTrailStructure::getMatched(unsigned int idx) {
    // check if the idx is present in the married structure
    if (married[0] == idx) return married[1];
    if (married[1] == idx) return married[0];
    if (married[2] == idx) return married[3];
    if (married[3] == idx) return married[2];

    if (!matched[idx]) return idx;  // has no match

    // get start idx for the dyck word
    unsigned int start = lastClosed + 1 == degree ? 0 : lastClosed + 1;

    while (!matched[start]) {  // start is the first opening bracket after the last one closed.
        if (start == degree - 1) {
            start = 0;
        } else {
            start += 1;
        }
    }

    unsigned int j = start;
    unsigned int p = 0;
    std::vector<unsigned int> stack(degree / 2);
    do {
        if (matched[j]) {  // only push matched index
            if (inAndOut[j]) {  // '('
                stack[p++] = j;
            } else {
                unsigned int i = stack[--p];
                if (idx == i) return j;
                if (idx == j) return i;
            }
        }

        // increment circular
        j = j == degree - 1 ? 0 : j + 1;
    } while (j != start);

    return idx;
}

unsigned int Sealib::SimpleTrailStructure::leave() {
    unsigned int u = getNextUnused();
    return u == 0 ? (unsigned int) -1 : unused[u];
}

unsigned int Sealib::SimpleTrailStructure::enter(unsigned int i) {
    i = i * 3 + 1;  // multiply index so it works with the actual array.

    if (flags[1]) {  // black node, should not be called here. something went wrong
        return (unsigned int) -1;
    }

    if (!flags[0]) flags[0] = 1;  // set to grey

    unsigned int prevLink = unused[i - 1];
    unsigned int nextLink = unused[i + 1];

    unsigned int temp;

    if (prevLink * 3 > i) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }

    inAndOut[unused[i]] = 1;
    if (temp == i) {  // no other element, this is last
        flags[1] = 1;
        // black now, unused is not needed anymore
        nextUnused = 0;
        unused.resize(0);
        unused.shrink_to_fit();
        return (unsigned int) -1;  // returns non-value
    }
    unused[temp + 1] += nextLink;

    if (nextLink * 3 + i > degree * 3) {  // circle around
        temp = (nextLink * 3) - (degree * 3) + i;
    } else {
        temp = i + nextLink * 3;
    }
    unused[temp - 1] += prevLink;
    // not needed, we flip twice since we take another edge out now
    // flags.at(2).flip(); //taking an arc flips the parity

    // not empty yet, continue
    matched[unused[i]] = 1;
    matched[unused[temp]] = 1;
    lastClosed = unused[temp];  // lastClosed element

    i = temp;
    prevLink = unused[i - 1];
    nextLink = unused[i + 1];

    if (prevLink * 3 > i) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }

    if (temp == i) {  // no other element, this is last
        flags[1] = 0;
        nextUnused = 0;

        // unused is not needed anymore
        unused.resize(0);
        unused.shrink_to_fit();
        return lastClosed;  // returns the leaver element, lastClosed
    }

    unused[temp + 1] += nextLink;

    if (nextLink * 3 + i > degree * 3) {  // circle around
        temp = (nextLink * 3) - (degree * 3) + i;
    } else {
        temp = i + nextLink * 3;
    }

    unused[temp - 1] += prevLink;



    // update nextUnused
    i = temp;
    prevLink = unused[i - 1];

    if (prevLink * 3 > i) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }

    nextUnused = temp;

    return lastClosed;  // returns the leaver element.
}

bool Sealib::SimpleTrailStructure::isBlack() {
    return flags[1];
}

bool Sealib::SimpleTrailStructure::isGrey() {
    return flags[0];
}

bool Sealib::SimpleTrailStructure::isEven() {
    return flags[2];
}

void Sealib::SimpleTrailStructure::marry(unsigned int i, unsigned int o) {
    if (married[0] == (unsigned int) -1) {  // first call of marry
        // unmatch previous matches
        unsigned int iMatch = getMatched(i);
        unsigned int oMatch = getMatched(o);
        matched[iMatch].flip();
        matched[i].flip();
        matched[oMatch].flip();
        matched[o].flip();

        married[0] = i;
        married[1] = o;
    } else {  // second call of marry, should be maximum
        // unmatch previous matches
        unsigned int iMatch = getMatched(i);
        unsigned int oMatch = getMatched(o);
        matched[iMatch].flip();
        matched[i].flip();
        matched[oMatch].flip();
        matched[o].flip();

        married[2] = i;
        married[3] = o;
    }
}
