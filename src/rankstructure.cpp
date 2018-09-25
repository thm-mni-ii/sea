#include <sealib/localranktable.h>
#include <sealib/rankstructure.h>
#include <iostream>
#include <cmath>
#include <utility>

unsigned long Sealib::RankStructure::rank(unsigned long k) const {
    if (k == 0 || k > maxRank) {
        return (unsigned long) -1;
    }
    unsigned long segmentIdx = (k - 1) / segmentLength;
    unsigned char segment = bitset.getBlock(segmentIdx);
    auto localIdx = static_cast<unsigned char>((k - 1) % segmentLength);
    return setBefore(segmentIdx) + LocalRankTable::getLocalRank(segment, localIdx);
}

Sealib::RankStructure::RankStructure(
    const Sealib::Bitset<unsigned char> &bitset_) :
    bitset(bitset_),
    segmentCount(static_cast<unsigned int>(bitset.size() / segmentLength)) {
    auto lastSeg = static_cast<unsigned char>((bitset.size() % segmentLength));

    if ((lastSeg != 0) && bitset.size() != 0) {
        segmentCount++;
    }

    maxRank =
        segmentCount == 0 ?
        0 : segmentLength * (lastSeg == 0 ? segmentCount : segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (unsigned int i = 0; i < segmentCount; i++) {
        unsigned char segment = bitset.getBlock(i);
        if (LocalRankTable::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if (segmentCount != 0) {
        setCountTable.reserve(segmentCount);
        unsigned int cnt = 0;
        for (unsigned long i = 0; i < segmentCount - 1; i++) {
            unsigned char segment = bitset.getBlock(i);
            cnt += LocalRankTable::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
}

Sealib::RankStructure::RankStructure() : bitset(0) {
}

unsigned int Sealib::RankStructure::getSegmentCount() const {
    return segmentCount;
}

unsigned int Sealib::RankStructure::setBefore(unsigned long segment) const {
    if (segment == 0) return 0;
    return setCountTable[segment - 1];
}

unsigned char Sealib::RankStructure::getSegmentLength() const {
    return segmentLength;
}
const Sealib::Bitset<unsigned char> &Sealib::RankStructure::getBitset() const {
    return bitset;
}
unsigned int Sealib::RankStructure::getMaxRank() const {
    return maxRank;
}
const std::vector<unsigned int> &Sealib::RankStructure::getSetCountTable() const {
    return setCountTable;
}
const std::vector<unsigned int> &Sealib::RankStructure::getNonEmptySegments() const {
    return nonEmptySegments;
}
unsigned long Sealib::RankStructure::size() const {
    return bitset.size();
}
Sealib::RankStructure::~RankStructure() = default;
