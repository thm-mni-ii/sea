#include "sealib/dictionary/rankstructure.h"
#include <cmath>
#include <utility>
#include "localranktable.h"

namespace Sealib {

template <class B>
uint64_t RankStructureBase<B>::rank(uint64_t k) const {
    if (k == 0 || k > maxRank) {
        return INVALID;
    }
    uint64_t segmentIdx = (k - 1) / segmentLength;
    uint8_t segment = bitset.getBlock(segmentIdx);
    auto localIdx = static_cast<uint8_t>((k - 1) % segmentLength);
    return setBefore(segmentIdx) +
           LocalRankTable::getLocalRank(segment, localIdx);
}

template <class B>
void RankStructureBase<B>::initialize() {
    auto lastSeg = static_cast<uint8_t>((bitset.size() % segmentLength));

    if ((lastSeg != 0) && bitset.size() != 0) {
        segmentCount++;
    }

    if (segmentCount == 0)
        maxRank = 0;
    else if (lastSeg == 0)
        maxRank = segmentLength * segmentCount + lastSeg;
    else
        maxRank = segmentLength * (segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (uint32_t i = 0; i < segmentCount; i++) {
        uint8_t segment = bitset.getBlock(i);
        if (LocalRankTable::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if (segmentCount != 0) {
        setCountTable.reserve(segmentCount);
        uint32_t cnt = 0;
        for (uint64_t i = 0; i < segmentCount - 1; i++) {
            uint8_t segment = bitset.getBlock(i);
            cnt += LocalRankTable::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
}

template class RankStructureBase<Bitset<uint8_t>>;
template class RankStructureBase<VariantBitset>;

}  // namespace Sealib
