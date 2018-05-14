//
// Created by jmeintrup on 06.03.18.
//

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <bitset>
#include "sealib/trailstructure.h"

Sealib::TrailStructure::TrailStructure(unsigned int _degree, LocalDyckTable& _table) : degree(_degree), table(_table) {
    if (degree > 0) {
        nextUnused = 1;
    } else {
        nextUnused = 0;
    }
    lastClosed = (unsigned int) - 1;

    inAndOut = boost::dynamic_bitset<>(degree);
    matched = boost::dynamic_bitset<>(degree);

    flags = boost::dynamic_bitset<>(3);
    if (degree % 2 == 0) {
        flags[2].flip();
    }  // set parity
    if (degree == 0) {
        flags[1].flip();
    }  // node with no edges is possible, set black

    married = nullptr;

    unused = static_cast<unsigned int *>(malloc(sizeof(unsigned int) * degree * 3));
    // [1][0][1] [1][1][1] [1][2][1] ... [1][degree-1][1]
    for (unsigned int i = 0; i < degree; i++) {
        unused[i * 3] = 1;
        unused[i * 3 + 1] = i;
        unused[i * 3 + 2] = 1;
    }
}



inline unsigned int Sealib::TrailStructure::getNextUnused() {
    if (flags[1]) {  // black node
        return (unsigned int) - 1;
    }

    if (!flags[0]) {
        // cout << "Set to grey\n";
        flags[0].flip();
    }  // set to grey
    unsigned int prevLink = unused[nextUnused-1];
    unsigned int nextLink = unused[nextUnused+1];

    unsigned int temp;

    if (prevLink*3 > nextUnused) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + nextUnused;
    } else {
        temp = nextUnused - prevLink * 3;
    }
    if (temp == nextUnused) {  // no other element, this is last
        unsigned int retVal = nextUnused;
        nextUnused = 0;
        flags[1].flip();
        return retVal;
    }
    unused[temp+1] += nextLink;

    if (nextLink*3 + nextUnused > degree * 3) {  // circle around
        temp = (nextLink * 3) - (degree * 3)  + nextUnused;
    } else {
        temp = nextUnused + nextLink * 3;
    }
    unused[temp-1] += prevLink;
    unsigned int retVal = nextUnused;
    nextUnused = temp;

    flags[2].flip();  // taking an arc flips the parity

    return retVal;
}

unsigned int Sealib::TrailStructure::getMatched(unsigned int idx) {
    // check if the idx is present in the married structure
    if (married!= nullptr) {
        if (married[0] == idx) return married[1];
        if (married[1] == idx) return married[0];
        if (married[2] == idx) return married[3];
        if (married[3] == idx) return married[2];
    }

    if (!matched[idx]) return idx;  // has no match
    // get start idx for the dyck word
    dyckStart = lastClosed + 1 == degree ? 0 : lastClosed + 1;

    while (!matched[dyckStart]) {  // start is the first opening bracket after the last one closed.
        if (dyckStart == degree - 1) {
            dyckStart = 0;
        } else {
            dyckStart+=1;
        }
    }

    unsigned int j = dyckStart;
    unsigned int p = 0;
    auto *stack = static_cast<unsigned int *>(malloc((sizeof(unsigned int) * degree / 2)));
    do {
        if (matched[j]) {  // only push matched index
            if (inAndOut[j]) {  // '('
                stack[p++] = j;
            } else {
                unsigned int i = stack[--p];
                if (idx == i) {
                    return j;
                }
                if (idx == j) {
                    return i;
                }
            }
        }
        // increment circular
        j = j == degree - 1 ? 0 : j + 1;
    } while (j != dyckStart);

    return idx;
}

unsigned int Sealib::TrailStructure::leave() {
    unsigned int u = getNextUnused();
    unsigned int retVal;
    if (u == 0) {
        retVal = (unsigned int) -1;
    } else {
        lastClosed = unused[u];
        retVal = unused[u];
    }

    if (flags[1]) {
        free(unused);
        initDyckStructures();
    }
    return retVal;
}

unsigned int Sealib::TrailStructure::enter(unsigned int i) {
    i = i*3+1; //multiply index so it works with the actual array.

    if (flags[1]) {//black node, should not be called here. something went wrong
        return (unsigned int) - 1;
    }

    if (!flags[0]) {
        flags[0].flip();
    }  // set to grey

    unsigned int prevLink = unused[i-1];
    unsigned int nextLink = unused[i+1];

    unsigned int temp;

    if (prevLink*3 > i) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }

    inAndOut[unused[i]].flip();
    if (temp == i) {  // no other element, this is last
        flags[1].flip();  // blacken it
        // black now, unused is not needed anymore
        free(unused);
        initDyckStructures();

        nextUnused = 0;
        return (unsigned int) - 1; // returns non-value
    }
    unused[temp+1] += nextLink;

    if (nextLink*3 + i > degree * 3) {  // circle around
        temp = (nextLink * 3) - (degree * 3)   + i;
    } else {
        temp = i + nextLink * 3;
    }
    unused[temp-1] += prevLink;

    // not empty yet, continue
    matched[unused[i]].flip();
    matched[unused[temp]].flip();
    lastClosed = unused[temp];  // lastClosed element

    i = temp;
    prevLink = unused[i-1];
    nextLink = unused[i+1];

    if (prevLink*3 > i) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }

    if (temp == i) {  // no other element, this is last
        flags[1].flip();  // should be 0 now
        nextUnused = 0;

        // unused is not needed anymore
        free(unused);
        initDyckStructures();

        return lastClosed;  // returns the leaver element, lastClosed
    }

    unused[temp+1] += nextLink;

    if (nextLink*3 + i > degree * 3) {  // circle around
        temp = (nextLink * 3) - (degree * 3)   + i;
    } else {
        temp = i + nextLink * 3;
    }

    unused[temp-1] += prevLink;

    // update nextUnused
    i = temp;
    prevLink = unused[i-1];

    if (prevLink*3 > i) {  // circle around
        temp = (degree * 3) - (prevLink * 3) + i;
    } else {
        temp = i - prevLink * 3;
    }

    nextUnused = temp;

    return lastClosed;  //returns the leaver element.
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
    for (unsigned int i =0; i < degree; i++) {
        if (getMatchedNew(i) == i && !inAndOut[i]) {
            return i;
        }
    }
    return (unsigned int) - 1;
}

bool Sealib::TrailStructure::isEndingArc(unsigned int i) {
    return (getMatchedNew(i) == i && inAndOut[i]);
}

void Sealib::TrailStructure::initDyckWord() {
    dyckWord = boost::dynamic_bitset<>(matched.count());  // only matched are part of dyckword
    dyckStart = lastClosed + 1 == degree ? 0 : lastClosed + 1;

    if (dyckWord.size() > 0) {
        while (!matched[dyckStart]) {  // start is the first opening bracket after the last one closed.
            if (dyckStart == degree - 1) {
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
            j = j == degree - 1 ? 0 : j + 1;
        } while (j != dyckStart);
    }
}

void Sealib::TrailStructure::initPioneerRankSelect() {
    auto segmentLength = static_cast<unsigned long>(std::log2(table.getEntries()));
    pioneerRankSelectBitSet = boost::dynamic_bitset<>(dyckWord.size());

    long segmentCount = dyckWord.size() / segmentLength;
    unsigned long lastSegmentLength = dyckWord.size() % segmentLength;

    for (unsigned int i = 0; i < segmentCount; i++) {
        unsigned long segment;
        unsigned long beg = segmentLength * i;
        boost::to_block_range(dyckWord, make_tuple(beg, segmentLength, std::ref(segment)));

        unsigned long leftPioneer = table[segment]->leftPioneer;
        unsigned long rightPioneer = table[segment]->rightPioneer;

        if (leftPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitSet[beg + leftPioneer]) {
                pioneerRankSelectBitSet[beg + leftPioneer].flip();

                unsigned long matchedPioneer = findMatchNaive(dyckWord, beg + leftPioneer);

                if (!pioneerRankSelectBitSet[matchedPioneer]) {
                    pioneerRankSelectBitSet[matchedPioneer].flip();
                }
            }
        }
        if (rightPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitSet[beg + rightPioneer]) {

                pioneerRankSelectBitSet[beg + rightPioneer].flip();

                unsigned long matchedPioneer = findMatchNaive(dyckWord, beg + rightPioneer);

                if (!pioneerRankSelectBitSet[matchedPioneer]) {
                    pioneerRankSelectBitSet[matchedPioneer].flip();
                }
            }
        }
    }

    if (lastSegmentLength % segmentLength != 0) {
        unsigned long segment;
        unsigned long beg = segmentLength * segmentCount;
        boost::to_block_range(dyckWord, make_tuple(beg, lastSegmentLength, std::ref(segment)));
        LocalDyckTable::Data *data = LocalDyckTable::calculateLocalData(segment, static_cast<unsigned char>(lastSegmentLength));

        unsigned long leftPioneer = data->leftPioneer;
        unsigned long rightPioneer = data->rightPioneer;


        if (leftPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitSet[beg + leftPioneer]) {
                pioneerRankSelectBitSet[beg + leftPioneer].flip();
                unsigned long matchedPioneer = findMatchNaive(dyckWord, beg + leftPioneer);

                if (!pioneerRankSelectBitSet[matchedPioneer]) {
                    pioneerRankSelectBitSet[matchedPioneer].flip();
                }
            }
        }
        if (rightPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitSet[beg + rightPioneer]) {
                pioneerRankSelectBitSet[beg + rightPioneer].flip();


                unsigned long matchedPioneer = findMatchNaive(dyckWord, beg + rightPioneer);

                if (!pioneerRankSelectBitSet[matchedPioneer]) {
                    pioneerRankSelectBitSet[matchedPioneer].flip();
                }
            }
        }

        delete data;
    }

    pioneerRankSelect = SimpleRankSelect(pioneerRankSelectBitSet);
}

void Sealib::TrailStructure::initDyckStructures() {
    initDyckWord();
    initPioneerRankSelect();
    initPioneerWord();
    initPioneerTable();
}

void Sealib::TrailStructure::initPioneerWord() {
    unsigned long rank = pioneerRankSelect.rank(pioneerRankSelectBitSet.size() - 1);
    if (rank == boost::dynamic_bitset<>::npos) rank = 0;
    pioneerDyckWord = boost::dynamic_bitset<>(rank);

    unsigned long idx = 0;
    unsigned long pioneerIndex = pioneerRankSelect.select(1);
    while (pioneerIndex != boost::dynamic_bitset<>::npos) {
        pioneerDyckWord[idx++] = dyckWord[pioneerIndex];
        pioneerIndex = pioneerRankSelectBitSet.find_next(pioneerIndex);
    }
}

void Sealib::TrailStructure::initPioneerTable() {
    pioneerTable = std::vector<unsigned int>(pioneerDyckWord.size(), (unsigned int) - 1);
    for (unsigned int i = 0; i < pioneerDyckWord.size(); i++) {
        if (pioneerTable[i] == (unsigned int) - 1) {
            unsigned int match = static_cast<unsigned int>(findMatchNaive(pioneerDyckWord, i));
            pioneerTable[i] = match;
            pioneerTable[match] = i;
        }
    }
}

unsigned long Sealib::TrailStructure::findMatchNaive(const boost::dynamic_bitset<> &word, unsigned long idx) {
    unsigned int j = 0;
    unsigned int p = 0;
    auto *stack = static_cast<unsigned int *>(malloc((sizeof(unsigned int) * word.size() / 2)));
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            unsigned int i = stack[--p];
            if (idx == i) {
                return j;
            }
            if (idx == j) {
                return i;
            }
        }
        j++;
    } while (j != word.size());

    return idx;
}

const boost::dynamic_bitset<> &Sealib::TrailStructure::getDyckWord() const {
    return dyckWord;
}

unsigned int Sealib::TrailStructure::getDyckStart() {
    return dyckStart;
}

unsigned int Sealib::TrailStructure::getMatchedNew(unsigned int idx) {
    auto segmentLength = static_cast<unsigned long>(std::log2(table.getEntries()));
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
        s = (s == (degree - 1)) ? 0 : s+1;
    }
    unsigned long lastSegment = dyckWord.size() / segmentLength;
    unsigned long lastSegmentLength = dyckWord.size() % segmentLength;

    unsigned long segmentId = dyckIdx / segmentLength;
    auto bSegmentIdx = static_cast<unsigned char>(dyckIdx % segmentLength);

    if (segmentId != lastSegment || lastSegmentLength == 0) {  // not last segment or last segment has normal size, it is certain we can get a subword of length segmentLength
        unsigned long segment;
        unsigned long beg = segmentId * segmentLength;
        boost::to_block_range(dyckWord, make_tuple(beg, segmentLength, std::ref(segment)));

        unsigned char localMatch = table[segment]->localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            unsigned long match = beg + localMatch;

            // translate idx in dyckword to actual idx
            s = dyckStart;
            unsigned int i = 0;
            while (i < match) {
                if (matched[s]) {
                    i++;
                }
                s = (s == (degree - 1)) ? 0 : s+1;
            }
            while (!matched[s]) {
                s = (s == (degree - 1)) ? 0 : s+1;
            }
            return s;
        }
    } else {
        unsigned long segment;
        unsigned long beg = lastSegment * segmentLength;
        boost::to_block_range(dyckWord, make_tuple(beg, lastSegmentLength, std::ref(segment)));

        unsigned char localMatch = table[segment]->localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx || localMatch >= lastSegmentLength) {  // ignore matches outside of range
            unsigned long match = beg + localMatch;

            // translate idx in dyckword to actual idx
            s = dyckStart;
            unsigned int i = 0;
            while (i < match) {
                if (matched[s]) {
                    i++;
                }
                s = (s == (degree - 1)) ? 0 : s+1;
            }
            while (!matched[s]) {
                s = (s == (degree - 1)) ? 0 : s+1;
            }
            return s;
        }
    }
    // match is not local
    if (dyckWord[dyckIdx]) {  // opening global
        unsigned long p_wp = pioneerRankSelect.rank(dyckIdx);
        unsigned long p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp));

        unsigned int pMatch_wp = static_cast<unsigned int>(findMatchNaive(pioneerDyckWord, p_wp - 1)); //pioneerTable[p_wp - 1];
        unsigned long pMatch = pioneerRankSelect.select(pMatch_wp + 1);

        unsigned long pSeg;
        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        unsigned long beg = pSegment * segmentLength;
        boost::to_block_range(dyckWord, make_tuple(beg, segmentLength, std::ref(pSeg)));

        int pDepth = table[pSeg]->localDepths[pSegmentIdx];
        int bDepth = table[pSeg]->localDepths[bSegmentIdx];

        int depthDiff = bDepth - pDepth;

        unsigned long pMatchSeg;
        unsigned long pMatchSegment = pMatch / segmentLength;
        unsigned long matchSegmentIdx = pMatch % segmentLength;


        if (pMatchSegment != lastSegment || matchSegmentIdx == 0) {  // not last segment or last segment has normal size, it is certain we can get a subword of length segmentLength
            beg = pMatchSegment * segmentLength;
            boost::to_block_range(dyckWord, make_tuple(beg, segmentLength, std::ref(pMatchSeg)));
            int pMatchDepth = table[pMatchSeg]->localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = table[pMatchSeg]->localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    unsigned long match = (pMatchSegment * segmentLength) + i;
                    // translate idx in dyckword to actual idx
                    s = dyckStart;
                    unsigned int k = 0;
                    while (k < match) {
                        if (matched[s]) {
                            k++;
                        }
                        s = (s == (degree - 1)) ? 0 : s+1;
                    }
                    while (!matched[s]) {
                        s = (s == (degree - 1)) ? 0 : s+1;
                    }
                    return s;
                }
            }
        } else {
            beg = lastSegment * segmentLength;
            boost::to_block_range(dyckWord, make_tuple(beg, lastSegmentLength, std::ref(pMatchSeg)));
            LocalDyckTable::Data *data = LocalDyckTable::calculateLocalData(pMatchSeg, static_cast<unsigned char>(lastSegmentLength));

            int pMatchDepth = data->localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = data->localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    delete data;
                    unsigned long match = (pMatchSegment * segmentLength) + i;

                    // translate idx in dyckword to actual idx
                    s = dyckStart;
                    unsigned int k = 0;
                    while (k < match) {
                        if (matched[s]) {
                            k++;
                        }
                        s = (s == (degree - 1)) ? 0 : s+1;
                    }
                    while (!matched[s]) {
                        s = (s == (degree - 1)) ? 0 : s+1;
                    }
                    return s;
                }
            }
            delete data;
        }
    } else {  // closing global
        unsigned long p_wp = pioneerRankSelect.rank(dyckIdx);
        unsigned long p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp));
        if (p < dyckIdx) {
            p_wp++;
            p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp));
        }

        unsigned int pMatch_wp = static_cast<unsigned int>(findMatchNaive(pioneerDyckWord, p_wp - 1));  //pioneerTable[p_wp - 1];
        unsigned long pMatch = pioneerRankSelect.select(pMatch_wp + 1);


        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;


        unsigned long pSeg;
        unsigned long beg;

        int pDepth;
        int bDepth;
        int depthDiff;

        if (pSegment != lastSegment || pSegmentIdx == 0) {  // not in last segment
            beg = pSegment * segmentLength;
            boost::to_block_range(dyckWord, make_tuple(beg, segmentLength, std::ref(pSeg)));

            pDepth = table[pSeg]->localDepths[pSegmentIdx];
            bDepth = table[pSeg]->localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        } else {
            beg = lastSegment * segmentLength;
            boost::to_block_range(dyckWord, make_tuple(beg, lastSegmentLength, std::ref(pSeg)));
            LocalDyckTable::Data *data = LocalDyckTable::calculateLocalData(pSeg, static_cast<unsigned char>(lastSegmentLength));

            pDepth = data->localDepths[pSegmentIdx];
            bDepth = data->localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
            delete data;
        }

        unsigned long pMatchSeg;
        unsigned long pMatchSegment = pMatch / segmentLength;
        unsigned long matchSegmentIdx = pMatch % segmentLength;

        beg = pMatchSegment * segmentLength;
        boost::to_block_range(dyckWord, make_tuple(beg, segmentLength, std::ref(pMatchSeg)));
        int pMatchDepth = table[pMatchSeg]->localDepths[matchSegmentIdx];
        for (unsigned long i = matchSegmentIdx; i < segmentLength; i++) {
            int candidateDepth = table[pMatchSeg]->localDepths[i];
            int candidateDepthDifference = candidateDepth - pMatchDepth;
            if (candidateDepthDifference == depthDiff) {  // i is match
                unsigned long match = (pMatchSegment * segmentLength) + i;

                // translate idx in dyckword to actual idx
                s = dyckStart;
                unsigned int k = 0;
                while (k < match) {
                    if (matched[s]) {
                        k++;
                    }
                    s = (s == (degree - 1)) ? 0 : s+1;
                }
                while (!matched[s]) {
                    s = (s == (degree - 1)) ? 0 : s+1;
                }
                return s;
            }
        }
    }
    return idx;
}

unsigned int Sealib::TrailStructure::getDegree() const {
    return degree;
}

const boost::dynamic_bitset<> &Sealib::TrailStructure::getInAndOut() const {
    return inAndOut;
}

const boost::dynamic_bitset<> &Sealib::TrailStructure::getMatchedBitset() const {
    return matched;
}

unsigned int Sealib::TrailStructure::getLastClosed() {
    return lastClosed;
}
