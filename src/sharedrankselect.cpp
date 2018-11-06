#include <sealib/sharedrankselect.h>
#include <sealib/localselecttable.h>
#include <iostream>
#include <utility>

template <typename BlockType>
Sealib::SharedRankSelect<BlockType>::SharedRankSelect(
    std::shared_ptr<const Sealib::Bitset<BlockType>> bitset_) :
    rankStructure(std::move(bitset_)),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

template <typename BlockType>
unsigned long Sealib::SharedRankSelect<BlockType>::select(unsigned long k) const {
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
    BlockType localSelect =
        LocalSelectTable<BlockType>::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

template <typename BlockType>
Sealib::SharedRankSelect<BlockType>::SharedRankSelect() = default;

template <typename BlockType>
std::shared_ptr<const Sealib::Bitset<BlockType>>
Sealib::SharedRankSelect<BlockType>::generateFirstInBlockBitSet(
    const SharedRankStructure<BlockType> &rs) {
    unsigned long size = rs.rank(rs.size());
    if (size == (unsigned long) -1) {
        size = 0;
    }
    std::shared_ptr<Bitset<BlockType>> firstInBlockBitSet(new Bitset<BlockType>(size));

    for (unsigned long i = 0; i < rs.getSegmentCount(); i++) {
        BlockType segment = rs.getBitset().getBlock(i);
        BlockType localFirst = LocalSelectTable<BlockType>::getLocalSelect(segment, 0);
        // has a local first, i.e. is not an empty segment
        if (localFirst != LocalSelectTable<BlockType>::npos) {
            // setBefore gives us the index in firstInBlockBitset
            unsigned int before = rs.setBefore(i);
            (*firstInBlockBitSet)[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

template <typename BlockType>
Sealib::SharedRankSelect<BlockType>::~SharedRankSelect() = default;

template <typename BlockType>
unsigned long Sealib::SharedRankSelect<BlockType>::rank(unsigned long k) const {
    return rankStructure.rank(k);
}
template<typename BlockType>
const Sealib::Bitset<BlockType>
&Sealib::SharedRankSelect<BlockType>::getBitset() const {
    return rankStructure.getBitset();
}

namespace Sealib {
template
class SharedRankSelect<unsigned char>;
template
class SharedRankSelect<unsigned short>;
}  // namespace Sealib
