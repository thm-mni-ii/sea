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

    unsigned long size() const;

    ~RankSelect();
    const Bitset<unsigned char> &getBitset() const;
};
}  // namespace Sealib
#endif  // SEALIB_RANKSELECT_H_
