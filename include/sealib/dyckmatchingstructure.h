#ifndef SEALIB_DYCKMATCHINGSTRUCTURE_H_
#define SEALIB_DYCKMATCHINGSTRUCTURE_H_

#include <sealib/bitset.h>
#include <iostream>

namespace Sealib {
class DyckMatchingStructure {
 public:
    static unsigned long getMatchNaive(
        const Sealib::Bitset<unsigned char> &word_,
        unsigned long idx);
  
    static unsigned long getMatchNaive(
        const Sealib::Bitset<unsigned char> &word,
        unsigned long idx, unsigned long skip);

    virtual unsigned long getMatch(unsigned long idx);
  
    explicit DyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_);
  
    const Sealib::Bitset<unsigned char> &getWord() const;
  
    static const Bitset<unsigned char> segmentizeWord(const Bitset<unsigned char> &word);

 protected:
    const Sealib::Bitset<unsigned char> word;
    static constexpr const unsigned char mSegmentLength = 7
};
}  // namespace Sealib
#endif  // SEALIB_DYCKMATCHINGSTRUCTURE_H_
