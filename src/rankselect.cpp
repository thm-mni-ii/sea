#include <sealib/rankselect.h>
#include <sealib/localselecttable.h>
#include <iostream>
#include <utility>

template <typename BlockType>
Sealib::RankSelect<BlockType>::RankSelect(
    const Sealib::Bitset<BlockType> &bitset_) :
    rankStructure(bitset_),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

template <typename BlockType>
unsigned long Sealib::RankSelect<BlockType>::select(unsigned long k) const {
    if (k == 0 || rankStructure.getSegmentCount() == 0) {
        return (unsigned long) -1;
    }
    unsigned long firstInSegmentRank = firstInSegment.rank(k);
    if (firstInSegmentRank == (unsigned long) -1) {
        return (unsigned long) -1;
    }
    unsigned long h = rankStructure.getNonEmptySegments()[firstInSegmentRank - 1];
    BlockType segment = rankStructure.getBitset().getBlock(h);
    auto localIndex = static_cast<BlockType>(k - rankStructure.setBefore(h) - 1);
    BlockType localSelect = LocalSelectTable<BlockType>::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

template <typename BlockType>
Sealib::RankSelect<BlockType>::RankSelect() = default;

template <typename BlockType>
const Sealib::Bitset<BlockType> Sealib::RankSelect<BlockType>::generateFirstInBlockBitSet(
    const RankStructure<BlockType> &rs) {
    unsigned long size = rs.rank(rs.size());
    if (size == (unsigned long) -1) {
        size = 0;
    }
    Bitset<BlockType> firstInBlockBitSet(size);

    for (unsigned long i = 0; i < rs.getSegmentCount(); i++) {
        BlockType segment = rs.getBitset().getBlock(i);
        BlockType localFirst = LocalSelectTable<BlockType>::getLocalSelect(segment, 0);
        if (localFirst != LocalSelectTable<BlockType>::npos) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            unsigned int before = rs.setBefore(i);
            firstInBlockBitSet[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

template <typename BlockType>
Sealib::RankSelect<BlockType>::~RankSelect() = default;

template <typename BlockType>
unsigned long Sealib::RankSelect<BlockType>::rank(unsigned long k) const {
    return rankStructure.rank(k);
}
template <typename BlockType>
unsigned long Sealib::RankSelect<BlockType>::size() const {
    return rankStructure.size();
}
template <typename BlockType>
const Sealib::Bitset<BlockType> &Sealib::RankSelect<BlockType>::getBitset() const {
    return rankStructure.getBitset();
}

namespace Sealib {
template
class RankSelect<unsigned char>;
template
class RankSelect<unsigned short>;
}  // namespace Sealib