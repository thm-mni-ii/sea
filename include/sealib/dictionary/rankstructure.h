#ifndef SEALIB_DICTIONARY_RANKSTRUCTURE_H_
#define SEALIB_DICTIONARY_RANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var) >> (pos)) & 1)

#include <memory>
#include <vector>
#include "sealib/collection/bitset.h"
#include "sealib/collection/variantbitset.h"

namespace Sealib {

template <class B>
class RankStructureBase;
typedef RankStructureBase<Bitset<uint8_t>> RankStructure;
typedef RankStructureBase<VariantBitset> VariantRankStructure;

/**
 * Space efficient RankStructure implementation.
 * @param B Bitset type to use
 * @author Johannes Meintrup
 */
template <class B>
class RankStructureBase {
 public:
    /**
     * @param _bitset bitset used for rank-select
     */
    template <class BR>
    explicit RankStructureBase(BR &&_bitset)
        : bitset(std::forward<BR>(_bitset)),
          segmentCount(static_cast<uint32_t>(bitset.size() / segmentLength)) {
        initialize();
    }

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    uint64_t rank(uint64_t k) const;

    const std::vector<uint32_t> &getSetCountTable() const {
        return setCountTable;
    }
    const std::vector<uint32_t> &getNonEmptySegments() const {
        return nonEmptySegments;
    }

    /**
     * @return size of the bitset
     */
    uint64_t size() const { return bitset.size(); }

    uint32_t getMaxRank() const { return maxRank; }

    /**
     * @return segment length
     */
    uint8_t getSegmentLength() const { return segmentLength; }

    /**
     * @return segment count
     */
    uint32_t getSegmentCount() const { return segmentCount; }

    /**
     * @return the bitset
     */
    B const &getBitset() const { return bitset; }

    uint32_t setBefore(uint64_t segment) const {
        return segment == 0 ? 0 : setCountTable[segment - 1];
    }

 protected:
    static constexpr const uint8_t segmentLength = 8;
    B const bitset;
    uint32_t segmentCount;
    uint32_t maxRank;

    std::vector<uint32_t> setCountTable;
    std::vector<uint32_t> nonEmptySegments;

    void initialize();
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_RANKSTRUCTURE_H_
