//
// Created by jmeintrup on 10.05.18.
//
#include <iostream>
#include <include/sealib/localselecttable.h>
#include "sealib/rankselect.h"

Sealib::RankSelect::RankSelect(std::unique_ptr<boost::dynamic_bitset<unsigned char>> bitsetPointer_) : Sealib::RankStructure(std::move(bitsetPointer_)), firstInSegment(nullptr){
    //initialize rank structure for firstInBlock
    unsigned long size = rank(bitsetPointer->size());

    auto firstInBlockBitSet = std::unique_ptr<boost::dynamic_bitset<unsigned char>>(new boost::dynamic_bitset<unsigned char>(size));

    for(unsigned long i = 0; i < segmentCount; i++) {
        unsigned char segment;
        boost::to_block_range(*bitsetPointer, std::make_tuple(i, std::ref(segment)));
        unsigned char localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (unsigned char) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            firstInBlockBitSet->operator[](setBefore(i)) = 1;
        }
    }
    firstInSegment = std::unique_ptr<RankStructure>(new RankStructure(std::move(firstInBlockBitSet)));
}


unsigned long Sealib::RankSelect::select(unsigned long k) const {
    if(k == 0 || segmentCount == 0) {
        return (unsigned long) - 1;
    }
    unsigned long firstInSegmentRank = firstInSegment->rank(k);
    if(firstInSegmentRank == (unsigned long) - 1) {
        return (unsigned long) - 1;
    }
    unsigned long h = nonEmptySegments[firstInSegmentRank - 1];
    unsigned char segment;
    boost::to_block_range(*bitsetPointer, std::make_tuple(h, std::ref(segment)));
    auto localIndex = static_cast<unsigned char>(k - setBefore(h) - 1);
    unsigned char localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + segmentLength * h + 1;
}

Sealib::RankSelect::RankSelect() : RankStructure(){

}
