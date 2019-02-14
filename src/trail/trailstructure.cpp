#include <sealib/collection/bitset.h>
#include <sealib/dictionary/rankselect.h>
#include "../dyck/recursivedyckmatchingstructure.h"
#include "smalldoublelinkedlist.h"
#include "largedoublelinkedlist.h"
#include "trailstructure.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

Sealib::TrailStructure::TrailStructure(uint32_t _degree) :
    lastClosed((uint32_t) -1),
    dyckStart((uint32_t) -1),
    inAndOut(_degree),
    matched(_degree),
    flags(4),
    dyckMatchingStructure(nullptr),
    married(nullptr),
    unused(nullptr) {
    if (_degree < 255) {
        unused.reset(new SmallDoubleLinkedList(static_cast<uint8_t>(_degree)));
    } else {
        unused.reset(new LargeDoubleLinkedList(_degree));
    }
    if (_degree % 2 == 0) {
        flags[2] = 1;
    }  // set parity
    if (_degree == 0) {
        flags[1] = 1;
        flags[2] = 1;
    }  // node with no edges is possible, set black
}

inline uint32_t Sealib::TrailStructure::getNextUnused() {
    if (flags[1]) {  // black node
        return (uint32_t) -1;
    }
    if (!flags[0]) {
        flags[0].flip();
    }  // set to grey

    uint32_t next = unused->get();
    if (unused->isEmpty()) {
        flags[1].flip();
        flags[2] = 1;
    } else {
        flags[2].flip();
    }
    return next;
}

uint32_t Sealib::TrailStructure::leave() {
    uint32_t u = getNextUnused();
    if (u != (uint32_t) -1) {
        lastClosed = u;
        flags[3] = 1;
    }
    if (flags[1]) {
        unused.release();
        initDyckStructure();
    }
    return u;
}

uint32_t Sealib::TrailStructure::enter(uint32_t i) {
    if (flags[1]) {
        return (uint32_t) -1;
    }
    uint32_t next = unused->remove(i);
    inAndOut[i] = 1;
    flags[2].flip();
    if (next == i) {  //  no elements left
        flags[1] = 1;  //  blacken it
        flags[2] = 1;
        //  black now, unused is not needed anymore
        unused.release();
        initDyckStructure();
        return (uint32_t) -1;
    }

    matched[i] = 1;
    matched[next] = 1;

    next = getNextUnused();
    lastClosed = next;
    if (flags[1]) {
        initDyckStructure();
    }

    return next;
}

bool Sealib::TrailStructure::isBlack() const {
    return flags[1];
}

bool Sealib::TrailStructure::isGrey() const {
    return flags[0];
}

bool Sealib::TrailStructure::isEven() const {
    return flags[2];
}

void Sealib::TrailStructure::marry(uint32_t i, uint32_t o) {
    // initialize married table if it's the first call
    if (married == nullptr) {
        married.reset(new std::vector<uint32_t>(4, (uint32_t) -1));

        uint32_t iMatch = getMatched(i);
        uint32_t oMatch = getMatched(o);
        matched[iMatch].flip();
        matched[i].flip();

        matched[oMatch].flip();
        matched[o].flip();

        (*married)[0] = i;
        (*married)[1] = o;
        (*married)[2] = (uint32_t) -1;
        (*married)[3] = (uint32_t) -1;
    } else {  // second call of marry, should be maximum
        uint32_t iMatch = getMatched(i);
        uint32_t oMatch = getMatched(o);

        matched[iMatch].flip();
        matched[i].flip();
        matched[oMatch].flip();
        matched[o].flip();

        (*married)[2] = i;
        (*married)[3] = o;
    }
}

uint32_t Sealib::TrailStructure::getStartingArc() const {
    if (!flags[3]) return (uint32_t) -1;

    for (uint32_t i = 0; i < inAndOut.size(); i++) {
        uint32_t match = getMatched(i);
        if (match == i && !inAndOut[i]) {
            return i;
        }
    }
    return (uint32_t) -1;
}

bool Sealib::TrailStructure::isEndingArc(uint32_t i) const {
    return (getMatched(i) == i && inAndOut[i]);
}

inline void Sealib::TrailStructure::initDyckStructure() {
    //  only matched indices are part of the dyckword
    uint64_t cnt = 0;
    for (uint64_t i = 0; i < matched.size(); i++) {
        if (matched[i]) cnt++;
    }

    Sealib::Bitset<uint8_t> dyckWord(cnt);
    dyckStart = lastClosed + 1 == inAndOut.size() ? 0 : lastClosed + 1;

    if (dyckWord.size() > 0) {
        // start is the first opening bracket after the last one closed.
        while (!matched[dyckStart]) {
            if (dyckStart == inAndOut.size() - 1) {
                dyckStart = 0;
            } else {
                dyckStart += 1;
            }
        }
        uint32_t j = dyckStart;
        uint32_t dyckIndex = 0;
        do {
            if (matched[j]) {  // only consider matched index
                dyckWord[dyckIndex++] = inAndOut[j];
            }
            // increment circular
            j = j == inAndOut.size() - 1 ? 0 : j + 1;
        } while (j != dyckStart);
        dyckMatchingStructure.reset(new RecursiveDyckMatchingStructure(dyckWord));
    }
}

uint32_t Sealib::TrailStructure::getDyckStart() const {
    return dyckStart;
}

uint32_t Sealib::TrailStructure::getMatchedNaive(uint32_t idx) {
    // check if the idx is present in the married structure
    if (married != nullptr) {
        if ((*married)[0] == idx) return (*married)[1];
        if ((*married)[1] == idx) return (*married)[0];
        if ((*married)[2] == idx) return (*married)[3];
        if ((*married)[3] == idx) return (*married)[2];
    }

    // has no match
    if (!matched[idx]) return idx;

    // get start idx for the dyck word
    uint32_t start = lastClosed + 1 == inAndOut.size() ? 0 : lastClosed + 1;

    // start is the first opening bracket after the last one closed.
    while (!matched[start]) {
        if (start == inAndOut.size() - 1) {
            start = 0;
        } else {
            start += 1;
        }
    }

    uint32_t j = start;
    uint32_t p = 0;
    std::vector<uint32_t> stack(inAndOut.size() / 2);
    do {
        // only push matched index
        if (matched[j]) {
            if (inAndOut[j]) {  // '('
                stack[p++] = j;
            } else {
                uint32_t i = stack[--p];
                if (idx == i) return j;
                if (idx == j) return i;
            }
        }

        // increment circular
        j = j == inAndOut.size() - 1 ? 0 : j + 1;
    } while (j != start);

    return idx;
}

uint32_t Sealib::TrailStructure::getMatched(uint32_t idx) const {
    // check if the idx is present in the married structure
    if (married != nullptr) {
        if ((*married)[0] == idx) return (*married)[1];
        if ((*married)[1] == idx) return (*married)[0];
        if ((*married)[2] == idx) return (*married)[3];
        if ((*married)[3] == idx) return (*married)[2];
    }

    if (!matched[idx]) return idx;  // has no match

    // calculate startidx of dyckword
    uint32_t dyckIdx = 0;
    uint32_t s = dyckStart;

    while (s != idx) {
        if (matched[s]) {
            dyckIdx++;
        }
        s = (s == (inAndOut.size() - 1)) ? 0 : s + 1;
    }

    uint64_t match = dyckMatchingStructure->getMatch(dyckIdx);
    if (match == dyckIdx) {
        return idx;
    }
    s = dyckStart;
    uint32_t i = 0;
    while (i < match) {
        if (matched[s]) {
            i++;
        }
        s = (s == (inAndOut.size() - 1)) ? 0 : s + 1;
    }
    while (!matched[s]) {
        s = (s == (inAndOut.size() - 1)) ? 0 : s + 1;
    }
    return s;
}

uint32_t Sealib::TrailStructure::getDegree() const {
    return static_cast<uint32_t>(inAndOut.size());
}

const Sealib::Bitset<uint8_t> &Sealib::TrailStructure::getInAndOut() const {
    return inAndOut;
}

const Sealib::Bitset<uint8_t> &Sealib::TrailStructure::getMatchedBitset() const {
    return matched;
}

uint32_t Sealib::TrailStructure::getLastClosed() const {
    return lastClosed;
}

bool Sealib::TrailStructure::hasStartingArc() const {
    return flags[3];
}
