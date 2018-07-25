#include <sealib/localranktable.h>
#include <sealib/sharedrankstructure.h>
#include <iostream>
#include <cmath>
#include <utility>


unsigned long Sealib::SharedRankStructure::rank(unsigned long k) const {
    if (k == 0 || k > maxRank) {
        return (unsigned long) -1;
    }
    unsigned long segmentIdx = (k - 1) / segmentLength;
    unsigned char segment = bitset->getBlock(segmentIdx);
    auto localIdx = static_cast<unsigned char>((k - 1) % segmentLength);
    return setBefore(segmentIdx) + LocalRankTable::getLocalRank(segment, localIdx);
}

Sealib::SharedRankStructure::SharedRankStructure(std::shared_ptr<Sealib::Bitset<unsigned char>> bitset_) :
    bitset(std::move(bitset_)),
    segmentCount(static_cast<unsigned int>(bitset->size() / segmentLength)) {
    auto lastSeg = static_cast<unsigned char>((bitset->size() % segmentLength));

    if ((lastSeg != 0) && bitset->size() != 0) {
        segmentCount++;
    }

    maxRank =
        segmentCount == 0 ?
        0 : segmentLength * (lastSeg == 0 ? segmentCount : segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (unsigned int i = 0; i < segmentCount; i++) {
        unsigned char segment = bitset->getBlock(i);
        if (LocalRankTable::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if (segmentCount != 0) {
        setCountTable.reserve(segmentCount);
        unsigned int cnt = 0;
        for (unsigned long i = 0; i < segmentCount - 1; i++) {
            unsigned char segment = bitset->getBlock(i);
            cnt += LocalRankTable::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
}

Sealib::SharedRankStructure::SharedRankStructure() : bitset(0) {
}

unsigned int Sealib::SharedRankStructure::getSegmentCount() const {
    return segmentCount;
}

unsigned int Sealib::SharedRankStructure::setBefore(unsigned long segment) const {
    if (segment == 0) return 0;
    return setCountTable[segment - 1];
}

unsigned char Sealib::SharedRankStructure::getSegmentLength() const {
    return segmentLength;
}
Sealib::SharedRankStructure::~SharedRankStructure() = default;
