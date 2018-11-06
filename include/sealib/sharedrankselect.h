#ifndef SEALIB_SHAREDRANKSELECT_H_
#define SEALIB_SHAREDRANKSELECT_H_

#include <sealib/sharedrankstructure.h>
#include <sealib/bitset.h>

namespace Sealib {
template <typename BlockType = unsigned char>
class SharedRankSelect;
}

namespace Sealib {
/**
* Space efficient RankSelect implementation.
* Uses a shared_ptr for the bitset, saves space if the bitset can be reused.
* @author Johannes Meintrup
*/
template <typename BlockType>
class SharedRankSelect {
 private:
    SharedRankStructure<BlockType> rankStructure;
    SharedRankStructure<BlockType> firstInSegment;
    static std::shared_ptr<const Bitset<BlockType>> generateFirstInBlockBitSet(
        const SharedRankStructure<BlockType> &sharedRankStructure);

 public:
    /**
     * @param shared_ptr managing the Sealib::Bitset used for RankSelect
     */
    explicit SharedRankSelect(std::shared_ptr<const Bitset<BlockType> > bitset);
    SharedRankSelect();

    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    unsigned long select(unsigned long k) const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    unsigned long rank(unsigned long k) const;

    ~SharedRankSelect();
    const Bitset<BlockType> &getBitset() const;
};
}  // namespace Sealib
#endif  // SEALIB_SHAREDRANKSELECT_H_
