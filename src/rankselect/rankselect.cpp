#include "sealib/dictionary/rankselect.h"
#include <utility>
#include "localselecttable.h"

namespace Sealib {

template <class B>
uint64_t RankSelectBase<B>::select(uint64_t k) const {
    if (k == 0 || rankStructure.getSegmentCount() == 0) {
        return INVALID;
    }
    uint64_t firstInSegmentRank = firstInSegment.rank(k);
    if (firstInSegmentRank == INVALID) {
        return INVALID;
    }
    uint64_t h = rankStructure.getNonEmptySegments()[firstInSegmentRank - 1];
    uint8_t segment = rankStructure.getBitset().getBlock(h);
    auto localIndex = static_cast<uint8_t>(k - rankStructure.setBefore(h) - 1);
    uint8_t localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

template <class B>
B RankSelectBase<B>::generateFirstInBlockBitSet(
    RankStructureBase<B> const &rs) {
    uint64_t size = rs.rank(rs.size());
    if (size == INVALID) {
        size = 0;
    }
    B firstInBlockBitSet(size);

    for (uint64_t i = 0; i < rs.getSegmentCount(); i++) {
        uint8_t segment = rs.getBitset().getBlock(i);
        uint8_t localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst !=
            (uint8_t)-1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            uint32_t before = rs.setBefore(i);
            firstInBlockBitSet[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

template class RankSelectBase<Bitset<uint8_t>>;
template class RankSelectBase<VariantBitset>;

}  // namespace Sealib
