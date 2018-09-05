#include <include/sealib/recursivedyckmatchingstructure.h>
#include <cmath>
#include <iostream>

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_, unsigned char recursions) :
    DyckMatchingStructure(word_),
    segments(static_cast<unsigned int>(word.size() / mSegmentLength)),
    lastSegment(static_cast<unsigned char>(word.size() % mSegmentLength)),
    pioneerMatchingStructure(nullptr),
    pioneerRankSelect(createPioneerRankSelectBitset()){

    unsigned long size = pioneerRankSelect.size();

    unsigned long maxRank = pioneerRankSelect.rank(size);
        if(maxRank > 0) {
            Sealib::Bitset<unsigned char> pioneerWord(maxRank);
            for(unsigned long i = 0; i < maxRank; i++) {
                unsigned long idx = pioneerRankSelect.select(i+1);
                pioneerWord[i] = word[idx - 1];
            }
            if(recursions > 0) {
                pioneerMatchingStructure = new Sealib::RecursiveDyckMatchingStructure(pioneerWord, static_cast<unsigned char>(recursions - 1));
            } else {
                pioneerMatchingStructure = new Sealib::DyckMatchingStructure(pioneerWord);
            }
    }
}

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_) : RecursiveDyckMatchingStructure(word_, 0) {
}

Sealib::Bitset<unsigned char> Sealib::RecursiveDyckMatchingStructure::createPioneerRankSelectBitset() {
    Sealib::Bitset<unsigned char> pioneerRankSelectBitset(word.size());

    for (unsigned int i = 0; i < segments; i++) {
        unsigned long beg = mSegmentLength * i;
        unsigned char segment = word.getBlock(i);

        unsigned long leftPioneer = LocalDyckTable::getLocalData(segment).leftPioneer;
        unsigned long rightPioneer = LocalDyckTable::getLocalData(segment).rightPioneer;

        if (leftPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitset[mSegmentLength * i + leftPioneer]) {
                pioneerRankSelectBitset[mSegmentLength * i + leftPioneer].flip();

                unsigned long matchedPioneer = DyckMatchingStructure::getMatch(beg + leftPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
        if (rightPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitset[mSegmentLength * i + rightPioneer]) {
                pioneerRankSelectBitset[mSegmentLength * i + rightPioneer].flip();

                unsigned long matchedPioneer = DyckMatchingStructure::getMatch(beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }
    if (lastSegment != 0) {
        unsigned char segment = word.getBlock(segments);

        for(unsigned int c = lastSegment; c < mSegmentLength; c+=2) {
            segment |= static_cast<unsigned char>(1 << c);
        }

        unsigned long rightPioneer = LocalDyckTable::getLocalData(segment).rightPioneer;

        if (rightPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitset[mSegmentLength * segments + rightPioneer]) {
                pioneerRankSelectBitset[mSegmentLength * segments + rightPioneer].flip();

                unsigned long matchedPioneer = DyckMatchingStructure::getMatch(mSegmentLength * segments + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }
    return pioneerRankSelectBitset;
}

unsigned long Sealib::RecursiveDyckMatchingStructure::getMatch(unsigned long idx) {
    unsigned long segmentId = idx / mSegmentLength;
    auto bSegmentIdx = static_cast<unsigned char>(idx % mSegmentLength);

    if(segmentId != segments || lastSegment == 0) {
        unsigned char segment = word.getBlock(segmentId);
        unsigned long beg = segmentId * mSegmentLength;

        unsigned char localMatch = LocalDyckTable::getLocalData(segment).localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    } else {
        unsigned char segment = word.getBlock(segments);
        unsigned long beg = segments * mSegmentLength;

        unsigned char localMatch = LocalDyckTable::getLocalData(segment).localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    }

    // match is not local
    if (word[idx]) {  // opening global
        unsigned long p_wp = pioneerRankSelect.rank(idx + 1);
        unsigned long p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp)) - 1;
        unsigned long pMatch_wp = pioneerMatchingStructure->getMatch(p_wp - 1);
        long pMatch = pioneerRankSelect.select(pMatch_wp + 1) - 1;


        unsigned long pSegment = p / mSegmentLength;
        unsigned long pSegmentIdx = p % mSegmentLength;

        unsigned char pSeg = word.getBlock(pSegment);

        int pDepth = LocalDyckTable::getLocalData(pSeg).localDepths[pSegmentIdx];
        int bDepth = LocalDyckTable::getLocalData(pSeg).localDepths[bSegmentIdx];

        int depthDiff = bDepth - pDepth;


        unsigned long pMatchSegment = static_cast<unsigned long>(pMatch / mSegmentLength);
        unsigned long matchSegmentIdx = static_cast<unsigned long>(pMatch % mSegmentLength);
        unsigned char pMatchSeg = word.getBlock(pMatchSegment);

        if (pMatch < mSegmentLength * segments) {  // not last segment or last segment has normal size, it is certain we can get a subword of length segmentLength
            int pMatchDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return (pMatchSegment * mSegmentLength) + i;
                }
            }
        } else {
            for(unsigned int c = lastSegment; c < mSegmentLength; c+=2) {
                pMatchSeg |= static_cast<unsigned char>(1 << c);
            }

            int pMatchDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return  (pMatchSegment * mSegmentLength) + i;
                }
            }
        }
    } else {  // closing global
        unsigned long p_wp = pioneerRankSelect.rank(idx + 1);
        unsigned long p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp)) - 1;
        if (p < idx) {
            p_wp++;
            p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp)) - 1;
        }
        unsigned long pMatch_wp = pioneerMatchingStructure->getMatch(p_wp - 1);
        unsigned long pMatch = pioneerRankSelect.select(pMatch_wp + 1) - 1;

        unsigned long pSegment = p / mSegmentLength;
        unsigned long pSegmentIdx = p % mSegmentLength;

        unsigned char pSeg;

        int pDepth;
        int bDepth;
        int depthDiff;

        if (pMatch < mSegmentLength * segments) {  // not in last segment

            pSeg = word.getBlock(pSegment);

            pDepth = LocalDyckTable::getLocalData(pSeg).localDepths[pSegmentIdx];
            bDepth = LocalDyckTable::getLocalData(pSeg).localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        } else {

            pSeg = word.getBlock(segments);

            for(unsigned int c = lastSegment; c < mSegmentLength; c+=2) {
                pSeg |= static_cast<unsigned char>(1 << c);
            }

            pDepth = LocalDyckTable::getLocalData(pSeg).localDepths[pSegmentIdx];
            bDepth = LocalDyckTable::getLocalData(pSeg).localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        }

        unsigned long pMatchSegment = pMatch / mSegmentLength;
        unsigned long matchSegmentIdx = pMatch % mSegmentLength;
        unsigned char pMatchSeg = word.getBlock(pMatchSegment);

        int pMatchDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
        for (unsigned long i = matchSegmentIdx; i < mSegmentLength; i++) {
            int candidateDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[i];
            int candidateDepthDifference = candidateDepth - pMatchDepth;
            if (candidateDepthDifference == depthDiff) {  // i is match
                return  (pMatchSegment * mSegmentLength) + i;
            }
        }
    }
    return idx;
}

