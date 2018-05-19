//
// Created by jmeintrup on 10.05.18.
//

#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include "sealib/rankselect.h"

Sealib::RankSelect::RankSelect(const boost::dynamic_bitset<> &bitset_) {
    segmentLength = log2(bitset_.size());

    // initialize local lookup tables
    unsigned long lastSeg = bitset_.size() % segmentLength;
    segmentCount = static_cast<unsigned int>(bitset_.size() / segmentLength);

    localRankLookupTable.reserve(segmentCount);
    localSelectLookupTable.reserve(segmentCount);

    nonEmptySegments.reserve(segmentCount);

    for (unsigned int i = 0; i < segmentCount; i++) {
        unsigned char rank = 0;
        std::vector<unsigned char> localRanks;
        std::vector<unsigned char> localSelect;

        localRanks.reserve(segmentLength);
        localSelect.reserve(segmentLength);
        unsigned long segment;
        unsigned long beg = i * segmentLength;
        boost::to_block_range(bitset_, make_tuple(beg, static_cast<unsigned long>(segmentLength), std::ref(segment)));

        for (unsigned char j = 0; j < segmentLength; j++) {
            if (CHECK_BIT(segment,j)) {
                localSelect.push_back(j);
                rank++;
            }
            localRanks.push_back(rank);
        }
        if (rank != 0) {
            nonEmptySegments.push_back(i);
        }
        localRankLookupTable.push_back(localRanks);
        localSelectLookupTable.push_back(localSelect);
    }

    if (lastSeg != 0) {
        unsigned char rank = 0;

        std::vector<unsigned char> localRanks;
        std::vector<unsigned char> localSelect;

        localRanks.reserve(lastSeg);
        localSelect.reserve(lastSeg);

        unsigned long segment;
        unsigned long beg = segmentCount * segmentLength;
        boost::to_block_range(bitset_, make_tuple(beg, lastSeg, std::ref(segment)));

        for (unsigned char j = 0; j < lastSeg; j++) {
            if (CHECK_BIT(segment,j)) {
                localSelect.push_back(j);
                rank++;
            }
            localRanks.push_back(rank);
        }
        if (rank != 0) {
            nonEmptySegments.push_back(segmentCount);
        }

        localRankLookupTable.push_back(localRanks);
        localSelectLookupTable.push_back(localSelect);

        segmentCount++;
    }

    // at(0) = number of set bits before the second block
    // at(segmentCount - 2) = number of bits set before the last block
    setCountTable.reserve(segmentCount);
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < segmentCount - 1; i++) {
        cnt += rankLocal(i, static_cast<unsigned char>(segmentLength - 1));
        setCountTable.push_back(cnt);
    }

    //initialize rank structure for firstInBlock
    unsigned long size = rank(bitset_.size());
    boost::dynamic_bitset<> firstInBlockBitSet = boost::dynamic_bitset<>(size);

    for(unsigned int i = 0; i < segmentCount; i++) {
        unsigned char localFirst = selectLocal(i, 0);
        if (localFirst != (unsigned char) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            firstInBlockBitSet[setBefore(i)] = 1;
        }
    }

    firstInSegment = RankStructure(firstInBlockBitSet);

    // has to be called or the assert(m_check_invariants()) fails
    // when the destructor for the firstInBlockBitSet is called
    firstInBlockBitSet.reset();
}

unsigned char Sealib::RankSelect::selectLocal(unsigned long segment, unsigned char localIdx) const {
    if (localSelectLookupTable[segment].size() <= localIdx) {
        return (unsigned char) - 1;
    }
    return localSelectLookupTable[segment][localIdx];
}

unsigned long Sealib::RankSelect::select(unsigned long k) const {
    unsigned long h = nonEmptySegments[firstInSegment.rank(k) - 1];
    unsigned char localIndex = static_cast<unsigned char>(k - setBefore(h) - 1);
    return selectLocal(h, localIndex) + segmentLength * h;
}