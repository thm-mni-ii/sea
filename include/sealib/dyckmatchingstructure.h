#ifndef SEALIB_DYCKMATCHINGSTRUCTURE_H_
#define SEALIB_DYCKMATCHINGSTRUCTURE_H_

#include <sealib/bitset.h>
#include <iostream>

namespace Sealib {

template <typename BlockType = unsigned char>
class DyckMatchingStructure;
}

namespace Sealib {
/**
 * Base class for dyckmatching.
 * Enables to find the match of a parenthesis in a dyck word w in a naive way, taking O(n) time.
 *
 * Also the static functions to find a match in a dyck word w in a naive way.
 */
template <typename BlockType>
class DyckMatchingStructure {
 public:
    /**
     * Finds the match of a parenthesis in a dyck word in O(n) using O(n) space at worst.
     * @param word_ - valid dyck word
     * @param idx - idx of the parenthesis in word_ for which the match should be found
     * @return match of word_(idx_), or idx_ if there is no match. (if word_ is not valid)
     */
    static unsigned long getMatchNaive(
        const Sealib::Bitset<BlockType> &word_,
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
    explicit DyckMatchingStructure(const Sealib::Bitset<BlockType> &word_);

    /**
     * Default descructor.
     */
    virtual ~DyckMatchingStructure();

    /**
     * @return Sealib::Bitset<unsigned char> word
     */
    const Sealib::Bitset<BlockType> &getWord() const;


 protected:
    const Sealib::Bitset<BlockType> word;
};
}  // namespace Sealib
#endif  // SEALIB_DYCKMATCHINGSTRUCTURE_H_
