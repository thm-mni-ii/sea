//
// Created by jmeintrup on 03.05.18.
//

#include <iostream>
#include <cmath>
#include <include/sealib/localranktable.h>
#include "sealib/rankstructure.h"
int Sealib::RankStructure::count = 0;

unsigned long Sealib::RankStructure::rank(unsigned long k) const{
    if(k == 0 || k > maxRank) {
        return (unsigned long) - 1;
    }
    unsigned long segmentIdx = (k - 1) / segmentLength;
    unsigned char segment;
    boost::to_block_range(*bitsetPointer, std::make_tuple(segmentIdx, std::ref(segment)));
    auto localIdx = static_cast<unsigned char>((k - 1) % segmentLength);
    return setBefore(segmentIdx) + LocalRankTable::getLocalRank(segment, localIdx);
}

Sealib::RankStructure::RankStructure(std::unique_ptr<boost::dynamic_bitset<unsigned char>> bitsetPointer_) :
        bitsetPointer(std::move(bitsetPointer_)),
        segmentCount(static_cast<unsigned int>(bitsetPointer->size() / segmentLength)){

    auto lastSeg = static_cast<unsigned char>((bitsetPointer->size() % segmentLength));

    if(lastSeg != 0 && !bitsetPointer->empty()) {
        segmentCount++;
    }

    maxRank = segmentCount == 0 ? 0 : segmentLength * (lastSeg == 0 ? segmentCount : segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (unsigned int i = 0; i < segmentCount; i++) {
        unsigned char segment;
        boost::to_block_range(*bitsetPointer, make_tuple(i, std::ref(segment)));
        if (LocalRankTable::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if(segmentCount != 0) {
        if(static_cast<unsigned char>((bitsetPointer->size() % segmentLength)) != 0) {
            segmentCount++;
        }

        setCountTable.reserve(segmentCount);
        unsigned int cnt = 0;
        for (unsigned long i = 0; i < segmentCount - 1; i++) {
            unsigned char segment;
            boost::to_block_range(*bitsetPointer, std::make_tuple(i, std::ref(segment)));
            cnt += LocalRankTable::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
    RankStructure::count++;
}


Sealib::RankStructure::RankStructure() {
    RankStructure::count++;
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

unsigned char Sealib::RankStructure::log2(unsigned long bits) {
    if(bits == 0) return 0;
    return static_cast<unsigned char>(64 - __builtin_clzl(bits));
}

