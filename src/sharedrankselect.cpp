#include <sealib/sharedrankselect.h>
#include <sealib/localselecttable.h>
#include <iostream>
#include <utility>

Sealib::SharedRankSelect::SharedRankSelect(
    std::shared_ptr<const Sealib::Bitset<unsigned char>> bitset_) :
    rankStructure(std::move(bitset_)),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

unsigned long Sealib::SharedRankSelect::select(unsigned long k) const {
    if (k == 0 || rankStructure.getSegmentCount() == 0) {
        return (unsigned long) -1;
    }
    unsigned long firstInSegmentRank = firstInSegment.rank(k);
    if (firstInSegmentRank == (unsigned long) -1) {
        return (unsigned long) -1;
    }
    unsigned long h = rankStructure.getNonEmptySegments()[firstInSegmentRank - 1];
    unsigned char segment = rankStructure.getBitset().getBlock(h);
    auto localIndex = static_cast<unsigned char>(k - rankStructure.setBefore(h) - 1);
    unsigned char localSelect = LocalSelectTable<>::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

Sealib::SharedRankSelect::SharedRankSelect() = default;

std::shared_ptr<const Sealib::Bitset<unsigned char>>
Sealib::SharedRankSelect::generateFirstInBlockBitSet(const SharedRankStructure &rs) {
    unsigned long size = rs.rank(rs.size());
    if (size == (unsigned long) -1) {
        size = 0;
    }
    std::shared_ptr<Bitset<unsigned char>> firstInBlockBitSet(new Bitset<unsigned char>(size));

    for (unsigned long i = 0; i < rs.getSegmentCount(); i++) {
        unsigned char segment = rs.getBitset().getBlock(i);
        unsigned char localFirst = LocalSelectTable<>::getLocalSelect(segment, 0);
        if (localFirst != (unsigned char) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            unsigned int before = rs.setBefore(i);
            (*firstInBlockBitSet)[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

Sealib::SharedRankSelect::~SharedRankSelect() = default;

unsigned long Sealib::SharedRankSelect::rank(unsigned long k) const {
    return rankStructure.rank(k);
}
