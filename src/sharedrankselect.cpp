//
// Created by jmeintrup on 10.05.18.
//
#include <iostream>
#include <include/sealib/localselecttable.h>
#include <unordered_set>
#include "sealib/sharedrankselect.h"

Sealib::SharedRankSelect::SharedRankSelect(std::shared_ptr<Sealib::Bitset<unsigned char>> bitset_) : Sealib::SharedRankStructure(bitset_), firstInSegment(nullptr){
    //initialize rank structure for firstInBlock
    unsigned long size = SharedRankStructure::rank(bitset->size());
    if(size == (unsigned long) - 1) {
        size = 0;
    }
    std::shared_ptr<Bitset<unsigned char>> firstInBlockBitSet(new Bitset<unsigned char>(size));

    for (unsigned long i = 0; i < segmentCount; i++) {
        unsigned char segment = bitset->getBlock(i);
        unsigned char localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (unsigned char) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            unsigned int before = setBefore(i);
            (*firstInBlockBitSet)[before] = 1;
        }
    }
    firstInSegment = new SharedRankStructure(firstInBlockBitSet);
}


unsigned long Sealib::SharedRankSelect::select(unsigned long k) const {
    if (k == 0 || segmentCount == 0) {
        return (unsigned long) - 1;
    }
    unsigned long firstInSegmentRank = firstInSegment->rank(k);
    if (firstInSegmentRank == (unsigned long) - 1) {
        return (unsigned long) - 1;
    }
    unsigned long h = nonEmptySegments[firstInSegmentRank - 1];
    unsigned char segment = bitset->getBlock(h);
    auto localIndex = static_cast<unsigned char>(k - setBefore(h) - 1);
    unsigned char localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + segmentLength * h + 1;
}

Sealib::SharedRankSelect::SharedRankSelect() : SharedRankStructure(){

}