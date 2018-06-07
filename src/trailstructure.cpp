//
// Created by jmeintrup on 06.03.18.
//

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <bitset>
#include <include/sealib/rankselect.h>
#include <include/sealib/recursivedyckmatchingstructure.h>
#include <include/sealib/smalldoublelinkedlist.h>
#include "sealib/trailstructure.h"

Sealib::TrailStructure::TrailStructure(unsigned int _degree) :
        lastClosed((unsigned int) - 1),
        inAndOut( _degree),
        matched(_degree),
        flags(3),
        married(nullptr),
        dl(nullptr) {
    if(_degree < 255) {
        dl = new SmallDoubleLinkedList(_degree);
    } else {
        dl = new LargeDoubleLinkedList(_degree);
    }
    if ( _degree % 2 == 0) {
        flags[2] = 1;
    }  // set parity
    if ( _degree == 0) {
        flags[1] = 1;
        flags[2] = 1;
    }  // node with no edges is possible, set black
}

inline unsigned int Sealib::TrailStructure::getNextUnused() {
    if (flags[1]) {  // black node
        return (unsigned int) - 1;
    }
    if (!flags[0]) {
        flags[0].flip();
    }  // set to grey

    unsigned int next = dl->get();
    if(dl->isEmpty()) {
        flags[1].flip();
        flags[2] = 1;
    } else {
        flags[2].flip();
    }
    return next;
}

unsigned int Sealib::TrailStructure::leave() {
    unsigned int u = getNextUnused();
    if(u != (unsigned int) -1) {
        lastClosed = u;
        flags[3] = 1;
    }
    if (flags[1]) {
        delete dl;
        initDyckStructure();
    }
    return u;
}

unsigned int Sealib::TrailStructure::enter(unsigned int i) {
    if(flags[1]) {
        return (unsigned int) - 1;
    }
    unsigned int next = dl->remove(i);
    inAndOut[i] = 1;
    flags[2].flip();
    if(next == i) {  // no elements left
        flags[3] = 1;  // has unmatched elements
        flags[1] = 1;  // blacken it
        flags[2] = 1;
        // black now, unused is not needed anymore
        delete dl;
        initDyckStructure();
        return (unsigned int) - 1;
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

bool Sealib::TrailStructure::isBlack() {
    return flags[1];
}

bool Sealib::TrailStructure::isGrey() {
    return flags[0];
}

bool Sealib::TrailStructure::isEven() {
    return flags[2];
}

void Sealib::TrailStructure::marry(unsigned int i, unsigned int o) {
    // initialize married table if it's the first call
    if (married == nullptr) {
        married = static_cast<unsigned int  *>(malloc(sizeof(unsigned int) * 4));
        married[0] = i;
        married[1] = o;
        married[2] = (unsigned int) - 1;
        married[3] = (unsigned int) - 1;
    } else {  // second call of marry, should be maximum
        married[2] = i;
        married[3] = o;
    }
}

unsigned int Sealib::TrailStructure::getStartingArc() {
    if(!flags[3]) return (unsigned int) - 1;

    for (unsigned int i =0; i < inAndOut.size(); i++) {
        unsigned int match = getMatched(i);
        if (match == i && !inAndOut[i]) {
            return i;
        }
    }
    return (unsigned int) - 1;
}

bool Sealib::TrailStructure::isEndingArc(unsigned int i) {
    return (getMatched(i) == i && inAndOut[i]);
}

inline void Sealib::TrailStructure::initDyckStructure() {
    boost::dynamic_bitset<unsigned char> dyckWord(matched.count());  // only matched are part of dyckword
    dyckStart = lastClosed + 1 == inAndOut.size() ? 0 : lastClosed + 1;

    if (dyckWord.size() > 0) {
        while (!matched[dyckStart]) {  // start is the first opening bracket after the last one closed.
            if (dyckStart == inAndOut.size() - 1) {
                dyckStart = 0;
            } else {
                dyckStart+=1;
            }
        }
        unsigned int j = dyckStart;
        unsigned int dyckIndex = 0;
        do {
            if (matched[j]) {  // only consider matched index
                dyckWord[dyckIndex++] = inAndOut[j];
            }
            // increment circular
            j = j == inAndOut.size() - 1 ? 0 : j + 1;
        } while (j != dyckStart);
        dyckMatchingStructure = new RecursiveDyckMatchingStructure(dyckWord);
    }
}

unsigned int Sealib::TrailStructure::getDyckStart() {
    return dyckStart;
}

unsigned int Sealib::TrailStructure::getMatchedNaive(unsigned int idx) {
    //check if the idx is present in the married structure
    if(married != nullptr) {
        if(married[0] == idx) return married[1];
        if(married[1] == idx) return married[0];
        if(married[2] == idx) return married[3];
        if(married[3] == idx) return married[2];
    }

    if(!matched[idx]) return idx; //has no match

    //get start idx for the dyck word
    unsigned int start = lastClosed + 1 == inAndOut.size() ? 0 : lastClosed + 1;

    while(!matched[start]) { //start is the first opening bracket after the last one closed.
        if(start == inAndOut.size() - 1) {
            start = 0;
        } else {
            start+=1;
        }
    }

    unsigned int j = start;
    unsigned int p = 0;
    unsigned int *stack = static_cast<unsigned int *>(malloc((sizeof(unsigned int) * inAndOut.size() / 2)));
    do {
        if(matched[j]) { //only push matched index
            if(inAndOut[j]) { // '('
                stack[p++] = j;
            } else {
                unsigned int i = stack[--p];
                if(idx == i) return j;
                if(idx == j) return i;
            }
        }

        //increment circular
        j = j == inAndOut.size() - 1 ? 0 : j + 1;

    } while(j != start);

    return idx;
}

unsigned int Sealib::TrailStructure::getMatched(unsigned int idx) {
    // check if the idx is present in the married structure
    if (married != nullptr) {
        if (married[0] == idx) return married[1];
        if (married[1] == idx) return married[0];
        if (married[2] == idx) return married[3];
        if (married[3] == idx) return married[2];
    }

    if (!matched[idx]) return idx;  // has no match

    // calculate startidx of dyckword
    unsigned int dyckIdx = 0;
    unsigned int s = dyckStart;

    while (s != idx) {
        if (matched[s]) {
            dyckIdx++;
        }
        s = (s == (inAndOut.size() - 1)) ? 0 : s+1;
    }
    unsigned long match = dyckMatchingStructure->findMatch(dyckIdx);
    if(match == dyckIdx) {
        return idx;
    }
    s = dyckStart;
    unsigned int i = 0;
    while (i < match) {
        if (matched[s]) {
            i++;
        }
        s = (s == (inAndOut.size() - 1)) ? 0 : s+1;
    }
    while (!matched[s]) {
        s = (s == (inAndOut.size() - 1)) ? 0 : s+1;
    }
    return s;
}

unsigned int Sealib::TrailStructure::getDegree() const {
    return static_cast<unsigned int>(inAndOut.size());
}

const boost::dynamic_bitset<unsigned char> &Sealib::TrailStructure::getInAndOut() const {
    return inAndOut;
}

const boost::dynamic_bitset<unsigned char> &Sealib::TrailStructure::getMatchedBitset() const {
    return matched;
}

unsigned int Sealib::TrailStructure::getLastClosed() {
    return lastClosed;
}

