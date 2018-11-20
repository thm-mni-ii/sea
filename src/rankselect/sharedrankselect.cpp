#include <sealib/sharedrankselect.h>
#include <sealib/localselecttable.h>
#include <iostream>
#include <utility>

Sealib::SharedRankSelect::SharedRankSelect(
    std::shared_ptr<const Sealib::Bitset<uint8_t>> bitset_) :
    rankStructure(std::move(bitset_)),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

uint64_t Sealib::SharedRankSelect::select(uint64_t k) const {
    if (k == 0 || rankStructure.getSegmentCount() == 0) {
        return (uint64_t) -1;
    }
    uint64_t firstInSegmentRank = firstInSegment.rank(k);
    if (firstInSegmentRank == (uint64_t) -1) {
        return (uint64_t) -1;
    }
    uint64_t h = rankStructure.getNonEmptySegments()[firstInSegmentRank - 1];
    uint8_t segment = rankStructure.getBitset().getBlock(h);
    auto localIndex = static_cast<uint8_t>(k - rankStructure.setBefore(h) - 1);
    uint8_t localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

Sealib::SharedRankSelect::SharedRankSelect() = default;

std::shared_ptr<const Sealib::Bitset<uint8_t>>
Sealib::SharedRankSelect::generateFirstInBlockBitSet(const SharedRankStructure &rs) {
    uint64_t size = rs.rank(rs.size());
    if (size == (uint64_t) -1) {
        size = 0;
    }
    std::shared_ptr<Bitset<uint8_t>> firstInBlockBitSet(new Bitset<uint8_t>(size));

    for (uint64_t i = 0; i < rs.getSegmentCount(); i++) {
        uint8_t segment = rs.getBitset().getBlock(i);
        uint8_t localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (uint8_t) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            uint32_t before = rs.setBefore(i);
            (*firstInBlockBitSet)[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

Sealib::SharedRankSelect::~SharedRankSelect() = default;

uint64_t Sealib::SharedRankSelect::rank(uint64_t k) const {
    return rankStructure.rank(k);
}
