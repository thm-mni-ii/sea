#ifndef SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_
#define SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_

#include <sealib/localdycktable.h>
#include <sealib/dyckmatchingstructure.h>
#include <sealib/rankselect.h>

namespace Sealib {
class RecursiveDyckMatchingStructure : public DyckMatchingStructure {
 public:
    explicit RecursiveDyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_,
                                            unsigned char recursions);

    explicit RecursiveDyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_);
    unsigned long getMatch(unsigned long idx) override;

 private:
    unsigned int segments;
    unsigned char lastSegment;
    DyckMatchingStructure *pioneerMatchingStructure;
    RankSelect pioneerRankSelect;

    Sealib::Bitset<unsigned char> createPioneerRankSelectBitset();
};
}  // namespace Sealib
#endif  // SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_
