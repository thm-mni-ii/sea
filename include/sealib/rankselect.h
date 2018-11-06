#ifndef SEALIB_RANKSELECT_H_
#define SEALIB_RANKSELECT_H_

#include <sealib/rankstructure.h>
#include <sealib/bitset.h>

namespace Sealib {
template <typename BlockType = unsigned char>
class RankSelect;
}
namespace Sealib {
/**
* Space efficient RankSelect implementation.
* @author Johannes Meintrup
*/
template <typename BlockType>
class RankSelect {
 private:
    RankStructure<BlockType> rankStructure;
    RankStructure<BlockType> firstInSegment;
    const Bitset<BlockType> generateFirstInBlockBitSet(
        const RankStructure<BlockType> &RankStructure);

 public:
    /**
     * @param Sealib::Bitset used for RankSelect
     */
    explicit RankSelect(const Bitset<BlockType> &bitset);
    RankSelect();

    /**
     * Selects the k-th set bit
     * @param k bit (in {1,...,B})
     * @return index of the k-th set bit (in {1,...,n})
     */
    unsigned long select(unsigned long k) const;

    /**
     * Rank of the k-th idx
     * @param k idx (in {1,...,n})
     * @return rank of k-th idx (in {0,...,B})
     */
    unsigned long rank(unsigned long k) const;

    unsigned long size() const;

    ~RankSelect();
    const Bitset<BlockType> &getBitset() const;
};
}  // namespace Sealib
#endif  // SEALIB_RANKSELECT_H_
