#ifndef SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_
#define SEALIB_RECURSIVEDYCKMATCHINGSTRUCTURE_H_

#include <sealib/localdycktable.h>
#include <sealib/dyckmatchingstructure.h>
#include <sealib/rankselect.h>
#include <map>

namespace Sealib {
/**
 * Space efficient implementation of a dyck matching structure.
 * Enables to find the match of a given parenthesis b in a dyckword w in constant time.
 */
class RecursiveDyckMatchingStructure : public DyckMatchingStructure {
 public:
    /**
     * Constructs the matching structure for the given dyck word word_
     * The structure is recursively built for the dyck word d_p consisting of the pioneers of word_
     * up to a maximum recursion depth
     * @param word_ - valid dyck word. validity is not being tested
     * @param recursions - maximum recursion depth
     */
    explicit RecursiveDyckMatchingStructure(
        const Sealib::Bitset<unsigned char> &word_,
        unsigned int recursions);

    /**
     * Constructs the matching structure for the given dyck word word_
     * The structure is recursively built for the dyck word d_p consisting of the pioneers of word_
     * up to a maximum recursion depth of two.
     * @param word_ - valid dyck word. validity is not being tested
     */
    explicit RecursiveDyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_);

    /**
     * finds the match of a parenthesis b in word in constant time
     * @param idx of the parenthesis b in word
     * @return idx of the match of b in word
     */
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
