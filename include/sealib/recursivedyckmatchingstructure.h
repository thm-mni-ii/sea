//
// Created by jmeintrup on 12.05.18.
//

#ifndef SEA_RECURSIVEDYCKMATCHINGSTRUCTURE_H
#define SEA_RECURSIVEDYCKMATCHINGSTRUCTURE_H

#include <map>
#include "localdycktable.h"
#include "dyckmatchingstructure.h"
#include "rankselect.h"

namespace Sealib {
class RecursiveDyckMatchingStructure : public DyckMatchingStructure{
 public:
    static std::map<unsigned char, Sealib::LocalDyckTable*> tables;

    static const Sealib::LocalDyckTable::Data* getTableEntry(unsigned char segmentSize, unsigned long segment);

    explicit RecursiveDyckMatchingStructure(const boost::dynamic_bitset<unsigned char> &word_, unsigned char recursions);

    explicit RecursiveDyckMatchingStructure(const boost::dynamic_bitset<unsigned char> &word_);
    unsigned long findMatch(unsigned long idx);
 private:
    static const unsigned char segmentLength = 7;
    unsigned int segments;
    unsigned char lastSegment;
    DyckMatchingStructure *pioneerMatchingStructure;
    RankSelect *pioneerRankSelect;

    void initializePioneerRankSelectBitset(boost::dynamic_bitset<unsigned char> &pioneerRankSelectBitset);
};
}  // namespace Sealib
#endif //SEA_RECURSIVEDYCKMATCHINGSTRUCTURE_H
