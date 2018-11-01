#ifndef SEALIB_RANKSELECT_H_
#define SEALIB_RANKSELECT_H_

#include <sealib/rankstructure.h>
#include <sealib/bitset.h>

namespace Sealib {
/**
* Space efficient RankSelect implementation.
* @author Johannes Meintrup
*/
class RankSelect {
 private:
    RankStructure rankStructure;
    RankStructure firstInSegment;
    const Bitset<unsigned char> generateFirstInBlockBitSet(
        const RankStructure &RankStructure);

 public:
    /**
     * @param Sealib::Bitset used for RankSelect
     */
    explicit RankSelect(const Bitset<unsigned char> &bitset);
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
    const Bitset<unsigned char> &getBitset() const;
};
}  // namespace Sealib
#endif  // SEALIB_RANKSELECT_H_
