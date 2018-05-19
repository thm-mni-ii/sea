//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/recursivedyckmatchingstructure.h>
#include <cmath>
#include <iostream>

#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))

std::map<unsigned char, Sealib::LocalDyckTable*> Sealib::RecursiveDyckMatchingStructure::tables = std::map<unsigned char, Sealib::LocalDyckTable*>();

inline const Sealib::LocalDyckTable::Data* Sealib::RecursiveDyckMatchingStructure::getTableEntry(unsigned char segmentSize, unsigned long segment) {
    if(Sealib::RecursiveDyckMatchingStructure::tables.find(segmentSize) != Sealib::RecursiveDyckMatchingStructure::tables.end()) {
        return Sealib::RecursiveDyckMatchingStructure::tables[segmentSize]->operator[](segment);
    } else {
        Sealib::RecursiveDyckMatchingStructure::tables.insert(std::make_pair(segmentSize, new Sealib::LocalDyckTable(segmentSize)));
        return Sealib::RecursiveDyckMatchingStructure::tables[segmentSize]->operator[](segment);
    };
}

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word_, unsigned char recursions) :
    DyckMatchingStructure(word_),
    segmentLength(RankSelect::log2(word.size())),
    segments(static_cast<unsigned int>(word.size() / segmentLength)),
    lastSegment(static_cast<unsigned char>(word.size() % segmentLength)),
    pioneerMatchingStructure(nullptr) {

    if(segmentLength % 2 == 0) {
        segmentLength++;
        lastSegment = static_cast<unsigned char>(word.size() % segmentLength);
    }


    boost::dynamic_bitset<> pioneerRankSelectBitset(word.size());
    initializePioneerRankSelectBitset(pioneerRankSelectBitset);


    pioneerRankSelect = new RankSelect(pioneerRankSelectBitset);
    unsigned long maxRank = pioneerRankSelect->rank(pioneerRankSelectBitset.size());
        if(maxRank > 0) {
            boost::dynamic_bitset<> pioneerWord(maxRank);
            for(unsigned long i = 0; i < maxRank; i++) {
                unsigned long idx = pioneerRankSelect->select(i+1);
                pioneerWord[i] = word[idx];
            }

            if(recursions > 0) {
                pioneerMatchingStructure = new Sealib::RecursiveDyckMatchingStructure(pioneerWord, static_cast<unsigned char>(recursions - 1));
            } else {
                pioneerMatchingStructure = new Sealib::DyckMatchingStructure(pioneerWord);
            }
    }


    pioneerRankSelectBitset.reset();
}

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word_) : RecursiveDyckMatchingStructure(word_, 0) {
}

void Sealib::RecursiveDyckMatchingStructure::initializePioneerRankSelectBitset(boost::dynamic_bitset<> &pioneerRankSelectBitset) {
    for (unsigned int i = 0; i < segments; i++) {
        unsigned long segment;
        unsigned long beg = segmentLength * i;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(segment)));

        unsigned long leftPioneer = getTableEntry(segmentLength, segment)->leftPioneer;
        unsigned long rightPioneer = getTableEntry(segmentLength, segment)->rightPioneer;

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

        unsigned long rightPioneer = getTableEntry(segmentLength, segment)->rightPioneer;

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

        unsigned char localMatch = getTableEntry(segmentLength, segment)->localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    } else {
        unsigned long segment;
        unsigned long beg = segments * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(lastSegment), std::ref(segment)));
        unsigned char localMatch = getTableEntry(segmentLength, segment)->localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    }

    // match is not local
    if (word[idx]) {  // opening global
        unsigned long p_wp = pioneerRankSelect->rank(idx + 1);
        unsigned long p = pioneerRankSelect->select(static_cast<unsigned int>(p_wp));
        unsigned long pMatch_wp = pioneerMatchingStructure->findMatch(p_wp - 1);
        long pMatch = pioneerRankSelect->select(pMatch_wp + 1);

        unsigned long pSeg;
        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        unsigned long beg = pSegment * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pSeg)));

        int pDepth = getTableEntry(segmentLength,pSeg)->localDepths[pSegmentIdx];
        int bDepth = getTableEntry(segmentLength,pSeg)->localDepths[bSegmentIdx];

        int depthDiff = bDepth - pDepth;

        unsigned long pMatchSeg;
        unsigned long pMatchSegment = static_cast<unsigned long>(pMatch / segmentLength);
        unsigned long matchSegmentIdx = static_cast<unsigned long>(pMatch % segmentLength);

        if (pMatchSegment != lastSegment || matchSegmentIdx == 0) {  // not last segment or last segment has normal size, it is certain we can get a subword of length segmentLength
            beg = pMatchSegment * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pMatchSeg)));
            int pMatchDepth = getTableEntry(segmentLength,pMatchSeg)->localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = getTableEntry(segmentLength,pMatchSeg)->localDepths[i];
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
            int pMatchDepth = getTableEntry(segmentLength,pMatchSeg)->localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth = getTableEntry(segmentLength,pMatchSeg)->localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return  (pMatchSegment * segmentLength) + i;
                }
            }
        }
    } else {  // closing global
        unsigned long p_wp = pioneerRankSelect->rank(idx + 1);
        unsigned long p = pioneerRankSelect->select(static_cast<unsigned int>(p_wp));
        if (p < idx) {
            p_wp++;
            p = pioneerRankSelect->select(static_cast<unsigned int>(p_wp));
        }
        unsigned long pMatch_wp = pioneerMatchingStructure->findMatch(p_wp - 1);
        unsigned long pMatch = pioneerRankSelect->select(pMatch_wp + 1);

        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        unsigned long pSeg;
        unsigned long beg;

        int pDepth;
        int bDepth;
        int depthDiff;

        if (pSegment != lastSegment || pSegmentIdx == 0) {  // not in last segment
            beg = pSegment * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pSeg)));

            pDepth = getTableEntry(segmentLength,pSeg)->localDepths[pSegmentIdx];
            bDepth = getTableEntry(segmentLength,pSeg)->localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        } else {
            beg = segments * segmentLength;
            boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(lastSegment), std::ref(pSeg)));
            for(unsigned int c = lastSegment; c < segmentLength; c+=2) {
                BIT_SET(pSeg, c);
            }

            pDepth = getTableEntry(segmentLength,pSeg)->localDepths[pSegmentIdx];
            bDepth = getTableEntry(segmentLength,pSeg)->localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        }
        unsigned long pMatchSeg;
        unsigned long pMatchSegment = pMatch / segmentLength;
        unsigned long matchSegmentIdx = pMatch % segmentLength;

        beg = pMatchSegment * segmentLength;
        boost::to_block_range(word, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(pMatchSeg)));
        int pMatchDepth = getTableEntry(segmentLength,pMatchSeg)->localDepths[matchSegmentIdx];
        for (unsigned long i = matchSegmentIdx; i < segmentLength; i++) {
            int candidateDepth = getTableEntry(segmentLength,pMatchSeg)->localDepths[i];
            int candidateDepthDifference = candidateDepth - pMatchDepth;
            if (candidateDepthDifference == depthDiff) {  // i is match
                return  (pMatchSegment * segmentLength) + i;
            }
        }
    }
    return idx;
}

