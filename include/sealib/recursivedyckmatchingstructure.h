#ifndef SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_
#define SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_


#include <sealib/localdycktable.h>
#include <sealib/dyckmatchingstructure.h>
#include <sealib/rankselect.h>
#include <map>

namespace Sealib {
class RecursiveDyckMatchingStructure : public DyckMatchingStructure {
 public:
    explicit RecursiveDyckMatchingStructure(
        const Sealib::Bitset<unsigned char> &word_,
        unsigned int recursions);

    explicit RecursiveDyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_);
    unsigned long getMatch(unsigned long idx) override;

 private:
    static const unsigned char segmentLength = 7;
    unsigned int segments;
    unsigned char lastSegment;
    RankSelect pioneerRankSelect;
    DyckMatchingStructure *pioneerMatchingStructure;

    const Sealib::Bitset<unsigned char> initializePioneerRankSelectBitset();
};
}  // namespace Sealib
#endif  // SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_
