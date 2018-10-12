#include <sealib/rankselect.h>
#include <sealib/localselecttable.h>
#include <iostream>
#include <utility>

Sealib::RankSelect::RankSelect(
    const Sealib::Bitset<unsigned char> &bitset_) :
    rankStructure(bitset_),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

unsigned long Sealib::RankSelect::select(unsigned long k) const {
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
    unsigned char localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

Sealib::RankSelect::RankSelect() = default;

const Sealib::Bitset<unsigned char> Sealib::RankSelect::generateFirstInBlockBitSet(
    const RankStructure &rs) {
    unsigned long size = rs.rank(rs.size());
    if (size == (unsigned long) -1) {
        size = 0;
    }
    Bitset<unsigned char> firstInBlockBitSet(size);

    for (unsigned long i = 0; i < rs.getSegmentCount(); i++) {
        unsigned char segment = rs.getBitset().getBlock(i);
        unsigned char localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (unsigned char) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            unsigned int before = rs.setBefore(i);
            firstInBlockBitSet[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

Sealib::RankSelect::~RankSelect() = default;

unsigned long Sealib::RankSelect::rank(unsigned long k) const {
    return rankStructure.rank(k);
}
unsigned long Sealib::RankSelect::size() const {
    return rankStructure.size();
}
