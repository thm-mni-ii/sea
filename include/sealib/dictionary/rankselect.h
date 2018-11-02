#ifndef SEALIB_RANKSELECT_H_
#define SEALIB_RANKSELECT_H_

#include <sealib/rankstructure.h>
#include <sealib/collection/bitset.h>

namespace Sealib {
/**
* Space efficient RankSelect implementation.
* @author Johannes Meintrup
*/
class RankSelect {
 private:
    RankStructure rankStructure;
    RankStructure firstInSegment;
    const Bitset<uint8_t> generateFirstInBlockBitSet(
        const RankStructure &RankStructure);

 public:
    /**
     * @param Sealib::Bitset used for RankSelect
     */
    explicit RankSelect(const Bitset<uint8_t> &bitset);
    RankSelect();

    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    uint64_t select(uint64_t k) const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    uint64_t rank(uint64_t k) const;

    uint64_t size() const;

    ~RankSelect();
    const Bitset<uint8_t> &getBitset() const;
};
}  // namespace Sealib
#endif  // SEALIB_RANKSELECT_H_
