//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/recursivedyckmatchingstructure.h>
#include <cmath>
#include <iostream>

#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))

inline const Sealib::LocalDyckTable::Data* Sealib::RecursiveDyckMatchingStructure::getTableEntry(unsigned long segment) {
    static Sealib::LocalDyckTable table(segmentLength);
    return table[segment];
}

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<unsigned char> &word_, unsigned char recursions) :
    DyckMatchingStructure(word_),
    segments(static_cast<unsigned int>(word.size() / segmentLength)),
    lastSegment(static_cast<unsigned char>(word.size() % segmentLength)),
    pioneerMatchingStructure(nullptr) {

    auto pioneerRankSelectBitset = std::unique_ptr<boost::dynamic_bitset<unsigned char>>(new boost::dynamic_bitset<unsigned char>(word.size()));
    initializePioneerRankSelectBitset(*pioneerRankSelectBitset);
    unsigned long size = pioneerRankSelectBitset->size();
    pioneerRankSelect = new RankSelect(std::move(pioneerRankSelectBitset));
    unsigned long maxRank = pioneerRankSelect->rank(size);
        if(maxRank > 0) {
            boost::dynamic_bitset<unsigned char> pioneerWord(maxRank);
            for(unsigned long i = 0; i < maxRank; i++) {
                unsigned long idx = pioneerRankSelect->select(i+1);
                pioneerWord[i] = word[idx - 1];
            }
            if(recursions > 0) {
                pioneerMatchingStructure = new Sealib::RecursiveDyckMatchingStructure(pioneerWord, static_cast<unsigned char>(recursions - 1));
            } else {
                pioneerMatchingStructure = new Sealib::DyckMatchingStructure(pioneerWord);
            }
    }
}

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<unsigned char> &word_) : RecursiveDyckMatchingStructure(word_, 0) {
}

void Sealib::RecursiveDyckMatchingStructure::initializePioneerRankSelectBitset(boost::dynamic_bitset<unsigned char> &pioneerRankSelectBitset) {
    for (unsigned int i = 0; i < segments; i++) {
        unsigned long segment;
        unsigned long beg = segmentLength * i;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(segment)));

        unsigned long leftPioneer = getTableEntry(segment)->leftPioneer;
        unsigned long rightPioneer = getTableEntry(segment)->rightPioneer;

        if (leftPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitset[beg + leftPioneer]) {
                pioneerRankSelectBitset[beg + leftPioneer].flip();

                unsigned long matchedPioneer = findMatchNaive(beg + leftPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
        if (rightPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitset[beg + rightPioneer]) {
                pioneerRankSelectBitset[beg + rightPioneer].flip();

                unsigned long matchedPioneer = findMatchNaive(beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }
    if (lastSegment != 0) {
        unsigned long segment;
        unsigned long beg = segmentLength * segments;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(lastSegment), std::ref(segment)));

        for(unsigned int c = lastSegment; c < segmentLength; c+=2) {
            BIT_SET(segment, c);
        }

        unsigned long rightPioneer = getTableEntry(segment)->rightPioneer;

        if (rightPioneer != (unsigned char) - 1) {
            if (!pioneerRankSelectBitset[beg + rightPioneer]) {
                pioneerRankSelectBitset[beg + rightPioneer].flip();

                unsigned long matchedPioneer = findMatchNaive(beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }
}

unsigned long Sealib::RecursiveDyckMatchingStructure::findMatch(unsigned long idx) {
    unsigned long segmentId = idx / segmentLength;
    auto bSegmentIdx = static_cast<unsigned char>(idx % segmentLength);

    if(segmentId != segments || lastSegment == 0) {
        unsigned long segment;
        unsigned long beg = segmentId * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(segment)));

        unsigned char localMatch = getTableEntry(segment)->localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    } else {
        unsigned long segment;
        unsigned long beg = segments * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(lastSegment), std::ref(segment)));
        unsigned char localMatch = getTableEntry(segment)->localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    }

    // match is not local
    if (word[idx]) {  // opening global
        unsigned long p_wp = pioneerRankSelect->rank(idx + 1);
        unsigned long p = pioneerRankSelect->select(static_cast<unsigned int>(p_wp)) - 1;
        unsigned long pMatch_wp = pioneerMatchingStructure->findMatch(p_wp - 1);
        long pMatch = pioneerRankSelect->select(pMatch_wp + 1) - 1;

        unsigned long pSeg;
        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        unsigned long beg = pSegment * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pSeg)));

        int pDepth = getTableEntry(pSeg)->localDepths[pSegmentIdx];
        int bDepth = getTableEntry(pSeg)->localDepths[bSegmentIdx];

        int depthDiff = bDepth - pDepth;

        unsigned long pMatchSeg;
        unsigned long pMatchSegment = static_cast<unsigned long>(pMatch / segmentLength);
        unsigned long matchSegmentIdx = static_cast<unsigned long>(pMatch % segmentLength);

        if (pMatch < segmentLength * segments) {  // not last segment or last segment has normal size, it is certain we can get a subword of length segmentLength
            beg = pMatchSegment * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pMatchSeg)));
            int pMatchDepth = getTableEntry(pMatchSeg)->localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = getTableEntry(pMatchSeg)->localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return (pMatchSegment * segmentLength) + i;
                }
            }
        } else {
            beg = segments * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(lastSegment), std::ref(pMatchSeg)));
            for(unsigned int c = lastSegment; c < segmentLength; c+=2) {
                BIT_SET(pMatchSeg, c);
            }
            int pMatchDepth = getTableEntry(pMatchSeg)->localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = getTableEntry(pMatchSeg)->localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return  (pMatchSegment * segmentLength) + i;
                }
            }
        }
    } else {  // closing global
        unsigned long p_wp = pioneerRankSelect->rank(idx + 1);
        unsigned long p = pioneerRankSelect->select(static_cast<unsigned int>(p_wp)) - 1;
        if (p < idx) {
            p_wp++;
            p = pioneerRankSelect->select(static_cast<unsigned int>(p_wp)) - 1;
        }
        unsigned long pMatch_wp = pioneerMatchingStructure->findMatch(p_wp - 1);
        unsigned long pMatch = pioneerRankSelect->select(pMatch_wp + 1) - 1;

        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        unsigned long pSeg;
        unsigned long beg;

        int pDepth;
        int bDepth;
        int depthDiff;

        if (pMatch < segmentLength * segments) {  // not in last segment
            beg = pSegment * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pSeg)));

            pDepth = getTableEntry(pSeg)->localDepths[pSegmentIdx];
            bDepth = getTableEntry(pSeg)->localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        } else {
            beg = segments * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(lastSegment), std::ref(pSeg)));
            for(unsigned int c = lastSegment; c < segmentLength; c+=2) {
                BIT_SET(pSeg, c);
            }

            pDepth = getTableEntry(pSeg)->localDepths[pSegmentIdx];
            bDepth = getTableEntry(pSeg)->localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        }
        unsigned long pMatchSeg;
        unsigned long pMatchSegment = pMatch / segmentLength;
        unsigned long matchSegmentIdx = pMatch % segmentLength;

        beg = pMatchSegment * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pMatchSeg)));
        int pMatchDepth = getTableEntry(pMatchSeg)->localDepths[matchSegmentIdx];
        for (unsigned long i = matchSegmentIdx; i < segmentLength; i++) {
            int candidateDepth = getTableEntry(pMatchSeg)->localDepths[i];
            int candidateDepthDifference = candidateDepth - pMatchDepth;
            if (candidateDepthDifference == depthDiff) {  // i is match
                return  (pMatchSegment * segmentLength) + i;
            }
        }
    }
    return idx;
}

