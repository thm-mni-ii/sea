#ifndef SEALIB_DYCKMATCHINGSTRUCTURE_H_
#define SEALIB_DYCKMATCHINGSTRUCTURE_H_

#include <sealib/bitset.h>
#include <iostream>

namespace Sealib {
class DyckMatchingStructure {
 public:
    unsigned long getMatchNaive(unsigned long idx);

    virtual unsigned long getMatch(unsigned long idx);

    explicit DyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_);

    const Sealib::Bitset<unsigned char> &getWord() const;

 protected:
    const Sealib::Bitset<unsigned char> word;
};
}  // namespace Sealib
#endif  // SEALIB_DYCKMATCHINGSTRUCTURE_H_
