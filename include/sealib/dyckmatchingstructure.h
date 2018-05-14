//
// Created by jmeintrup on 12.05.18.
//

#ifndef SEA_DYCKMATCHINGSTRUCTURE_H
#define SEA_DYCKMATCHINGSTRUCTURE_H

#include <include/sealib/dynamicbitsetextension.h>

class DyckMatchingStructure {
public:

    static unsigned long findMatchNaive(boost::dynamic_bitset<> &word_, unsigned long idx);

    explicit DyckMatchingStructure(const boost::dynamic_bitset<> &word_);
    const boost::dynamic_bitset<> &getWord() const;

 protected:
    boost::dynamic_bitset<> word;
};
#endif //SEA_DYCKMATCHINGSTRUCTURE_H
