#include <sealib/localranktable.h>
#include <sealib/rankstructure.h>
#include <iostream>
#include <cmath>
#include <utility>

template <typename BlockType>
unsigned long Sealib::RankStructure<BlockType>::rank(unsigned long k) const {
    if (k == 0 || k > maxRank) {
        return (unsigned long) -1;
    }
    unsigned long segmentIdx = (k - 1) / segmentLength;
    BlockType segment = bitset.getBlock(segmentIdx);
    auto localIdx = static_cast<BlockType>((k - 1) % segmentLength);
    return setBefore(segmentIdx) + LocalRankTable<BlockType>::getLocalRank(segment, localIdx);
}

template <typename BlockType>
Sealib::RankStructure<BlockType>::RankStructure(
    const Sealib::Bitset<BlockType> &bitset_) :
    bitset(bitset_),
    segmentCount(static_cast<unsigned int>(bitset.size() / segmentLength)) {
    auto lastSeg = static_cast<BlockType>((bitset.size() % segmentLength));

    if ((lastSeg != 0) && bitset.size() != 0) {
        segmentCount++;
    }

    maxRank =
        segmentCount == 0 ?
        0 : segmentLength * (lastSeg == 0 ? segmentCount : segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (unsigned int i = 0; i < segmentCount; i++) {
        BlockType segment = bitset.getBlock(i);
        if (LocalRankTable<BlockType>::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if (segmentCount != 0) {
        setCountTable.reserve(segmentCount);
        unsigned int cnt = 0;
        for (unsigned long i = 0; i < segmentCount - 1; i++) {
            BlockType segment = bitset.getBlock(i);
            cnt += LocalRankTable<BlockType>::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
}
template <typename BlockType>
Sealib::RankStructure<BlockType>::RankStructure() : bitset(0) {
}
template <typename BlockType>
unsigned int Sealib::RankStructure<BlockType>::getSegmentCount() const {
    return segmentCount;
}
template <typename BlockType>
unsigned int Sealib::RankStructure<BlockType>::setBefore(unsigned long segment) const {
    if (segment == 0) return 0;
    return setCountTable[segment - 1];
}
template <typename BlockType>
unsigned char Sealib::RankStructure<BlockType>::getSegmentLength() const {
    return segmentLength;
}
template <typename BlockType>
const Sealib::Bitset<BlockType> &Sealib::RankStructure<BlockType>::getBitset() const {
    return bitset;
}
template <typename BlockType>
unsigned int Sealib::RankStructure<BlockType>::getMaxRank() const {
    return maxRank;
}
template <typename BlockType>
const std::vector<unsigned int> &Sealib::RankStructure<BlockType>::getSetCountTable() const {
    return setCountTable;
}
template <typename BlockType>
const std::vector<unsigned int> &Sealib::RankStructure<BlockType>::getNonEmptySegments() const {
    return nonEmptySegments;
}
template <typename BlockType>
unsigned long Sealib::RankStructure<BlockType>::size() const {
    return bitset.size();
}
template <typename BlockType>
Sealib::RankStructure<BlockType>::~RankStructure() = default;

namespace Sealib {
template
class RankStructure<unsigned char>;
template
class RankStructure<unsigned short>;
}  // namespace Sealib
