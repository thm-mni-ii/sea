#ifndef SEALIB_DICTIONARY_RANKSELECT_H_
#define SEALIB_DICTIONARY_RANKSELECT_H_

#include "sealib/collection/bitset.h"
#include "sealib/collection/variantbitset.h"
#include "sealib/dictionary/rankstructure.h"

namespace Sealib {

template <class B>
class RankSelectBase;
typedef RankSelectBase<Bitset<uint8_t>> RankSelect;
typedef RankSelectBase<VariantBitset> VariantRankSelect;

/**
* Space efficient RankSelect implementation.
* @param B Bitset type to use
* @author Johannes Meintrup
*/
template <class B>
class RankSelectBase {
 public:
    /**
     * @param Sealib::Bitset used for RankSelect
     */
    template <class BR>
    explicit RankSelectBase(BR &&bitset)
        : rankStructure(std::forward<BR>(bitset)),
          firstInSegment(generateFirstInBlockBitSet(rankStructure)) {}

    /**
     * Selects the k-th set bit
     * @param k bit (in {1,...,B})
     * @return index of the k-th set bit (in {1,...,n})
     */
    uint64_t select(uint64_t k) const;

    /**
     * Rank of the k-th idx
     * @param k idx (in {1,...,n})
     * @return rank of k-th idx (in {0,...,B})
     */
    uint64_t rank(uint64_t k) const { return rankStructure.rank(k); }

    uint64_t size() const { return rankStructure.size(); }

    B const &getBitset() const { return rankStructure.getBitset(); }

 private:
    RankStructureBase<B> rankStructure;
    RankStructureBase<B> firstInSegment;
    B generateFirstInBlockBitSet(RankStructureBase<B> const &);
};

}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_RANKSELECT_H_
