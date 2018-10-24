#ifndef SEALIB_DYCKMATCHINGSTRUCTURE_H_
#define SEALIB_DYCKMATCHINGSTRUCTURE_H_

#include <sealib/bitset.h>
#include <iostream>

namespace Sealib {
/**
 * Base class for dyckmatching.
 * Enables to find the match of a parenthesis in a dyck word w in a naive way, taking O(n) time.
 *
 * Also the static functions to find a match in a dyck word w in a naive way.
 */
class DyckMatchingStructure {
 public:
    /**
     * Finds the match of a parenthesis in a dyck word in O(n) using O(n) space at worst.
     * @param word_ - valid dyck word
     * @param idx - idx of the parenthesis in word_ for which the match should be found
     * @return match of word_(idx_), or idx_ if there is no match. (if word_ is not valid)
     */
    static unsigned long getMatchNaive(
        const Sealib::Bitset<unsigned char> &word_,
        unsigned long idx);

    /**
     * Finds the match of a parenthesis in a dyck word.
     * @param idx - idx of the parenthesis in word_ for which the match should be found
     * @return match of word_(idx_), or idx_ if there is no match. (if word_ is not valid)
     */
    virtual unsigned long getMatch(unsigned long idx);

    /**
     * constructs a matching structure for the given dyck word word_.
     * There is no test to check if word_ is a valid dyck word.
     * @param word_ - dyck word for the matching structure
     */
    explicit DyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_);

    /**
     * @return Sealib::Bitset<unsigned char> word
     */
    const Sealib::Bitset<unsigned char> &getWord() const;


 protected:
    const Sealib::Bitset<unsigned char> word;
    static constexpr const unsigned char mSegmentLength = 7;
};
}  // namespace Sealib
#endif  // SEALIB_DYCKMATCHINGSTRUCTURE_H_
