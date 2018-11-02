#include <sealib/dictionary/rankselect.h>
#include <sealib/localselecttable.h>
#include <iostream>
#include <utility>

Sealib::RankSelect::RankSelect(
    const Sealib::Bitset<uint8_t> &bitset_) :
    rankStructure(bitset_),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

uint64_t Sealib::RankSelect::select(uint64_t k) const {
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

Sealib::RankSelect::RankSelect() = default;

const Sealib::Bitset<uint8_t> Sealib::RankSelect::generateFirstInBlockBitSet(
    const RankStructure &rs) {
    uint64_t size = rs.rank(rs.size());
    if (size == (uint64_t) -1) {
        size = 0;
    }
    Bitset<uint8_t> firstInBlockBitSet(size);

    for (uint64_t i = 0; i < rs.getSegmentCount(); i++) {
        uint8_t segment = rs.getBitset().getBlock(i);
        uint8_t localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (uint8_t) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            uint32_t before = rs.setBefore(i);
            firstInBlockBitSet[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

Sealib::RankSelect::~RankSelect() = default;

uint64_t Sealib::RankSelect::rank(uint64_t k) const {
    return rankStructure.rank(k);
}
uint64_t Sealib::RankSelect::size() const {
    return rankStructure.size();
}
const Sealib::Bitset<uint8_t> &Sealib::RankSelect::getBitset() const {
    return rankStructure.getBitset();
}
