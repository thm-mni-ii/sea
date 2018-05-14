//
// Created by jmeintrup on 12.05.18.
//

#ifndef SEA_RECURSIVEDYCKMATCHINGSTRUCTURE_H
#define SEA_RECURSIVEDYCKMATCHINGSTRUCTURE_H

#include <map>
#include "localdycktable.h"
#include "dyckmatchingstructure.h"

class RecursiveDyckMatchingStructure : public DyckMatchingStructure{
 public:
    static std::map<unsigned char, Sealib::LocalDyckTable> tables;

    static unsigned long findMatchNaive(boost::dynamic_bitset<> &word, unsigned long idx);
    static const Sealib::LocalDyckTable& getOrCreateTable(unsigned char segmentSize);
    static void initializePioneerRankSelectBitset(const boost::dynamic_bitset<> &word_, boost::dynamic_bitset<> &pioneerRankSelectBitset);

    explicit RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word_, bool last);

    explicit RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word_);
 private:
    boost::dynamic_bitset<> word;
    unsigned char segmentLength;

    DyckMatchingStructure *pioneerMatchingStructure;
};

#endif //SEA_RECURSIVEDYCKMATCHINGSTRUCTURE_H
