//
// Created by jmeintrup on 03.05.18.
//

#include <iostream>
#include <cmath>
#include "sealib/rankstructure.h"

unsigned long Sealib::RankStructure::rank(unsigned long k) const{
    unsigned long segmentIdx = (k - 1) / segmentLength;
    unsigned char localIdx = static_cast<unsigned char>((k - 1) % segmentLength);

    return setBefore(segmentIdx) + rankLocal(segmentIdx, localIdx);
}

Sealib::RankStructure::RankStructure(const boost::dynamic_bitset<> &bitset_) {
    segmentLength = RankStructure::log2(bitset_.size());

    // initialize local lookup tables
    unsigned long lastSeg = segmentLength == 0 ? 0 : bitset_.size() % segmentLength;
    segmentCount = segmentLength == 0 ? 0 : static_cast<unsigned int>(bitset_.size() / segmentLength);

    localRankLookupTable.reserve(segmentCount);

    nonEmptySegments.reserve(segmentCount);

    for (unsigned int i = 0; i < segmentCount; i++) {
        unsigned char rank = 0;
        std::vector<unsigned char> localRanks;

        localRanks.reserve(segmentLength);
        unsigned long segment;
        unsigned long beg = i * segmentLength;
        boost::to_block_range(bitset_, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(segment)));

        for (unsigned char j = 0; j < segmentLength; j++) {
            if (CHECK_BIT(segment,j)) {
                rank++;
            }
            localRanks.push_back(rank);
        }
        if (rank != 0) {
            nonEmptySegments.push_back(i);
        }
        localRankLookupTable.push_back(localRanks);
    }


    if (lastSeg != 0) {
        unsigned char rank = 0;

        std::vector<unsigned char> localRanks;

        localRanks.reserve(lastSeg);

        unsigned long segment;
        unsigned long beg = segmentCount * segmentLength;
        boost::to_block_range(bitset_, make_tuple(beg, lastSeg, std::ref(segment)));

        for (unsigned char j = 0; j < lastSeg; j++) {
            if (CHECK_BIT(segment,j)) {
                rank++;
            }
            localRanks.push_back(rank);
        }
        if(rank != 0) {
            nonEmptySegments.push_back(segmentCount);
        }

        localRankLookupTable.push_back(localRanks);
        segmentCount++;
    }

    if(segmentCount != 0) {
        // at(0) = number of set bits before the second block
        // at(segmentCount - 2) = number of bits set before the last block
        setCountTable.reserve(segmentCount);
        unsigned int cnt = 0;
        for (unsigned int i = 0; i < segmentCount - 1; i++) {
            cnt += rankLocal(i, static_cast<unsigned char>(segmentLength - 1));
            setCountTable.push_back(cnt);
        }
    }
}


Sealib::RankStructure::RankStructure() {
}

unsigned int Sealib::RankStructure::getSegmentCount() const {
    return segmentCount;
}

unsigned int Sealib::RankStructure::setBefore(unsigned long segment) const {
    if(segment == 0) return 0;
    return setCountTable[segment - 1];
}

unsigned char Sealib::RankStructure::getSegmentLength() const {
    return segmentLength;
}

 unsigned char Sealib::RankStructure::rankLocal(unsigned long segment, unsigned char localIdx) const {
    return localRankLookupTable[segment][localIdx];
}

inline unsigned char Sealib::RankStructure::log2(unsigned long bits) {
    if(bits == 0) return 0;
    return static_cast<unsigned char>(64 - __builtin_clzl(bits));
}

