#ifndef SEALIB_SHAREDRANKSTRUCTURE_H_
#define SEALIB_SHAREDRANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

#include <sealib/collection/bitset.h>
#include <memory>
#include <vector>

/**
 * Space efficient RankStructure implementation.
 * Uses a shared_ptr for the bitset, saves space if the bitset can be reused.
 * @author Johannes Meintrup
 */
namespace Sealib {
class SharedRankStructure {
 protected:
    static const uint8_t segmentLength = 8;
    std::shared_ptr<const Sealib::Bitset<uint8_t>> bitset;
    uint32_t segmentCount;
    uint32_t maxRank;

    std::vector<uint32_t> setCountTable;
    std::vector<uint32_t> nonEmptySegments;

 public:
    uint32_t getMaxRank() const;
    const std::vector<uint32_t> &getSetCountTable() const;
    const std::vector<uint32_t> &getNonEmptySegments() const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    uint64_t rank(uint64_t k) const;

    /**
     * @param shared_ptr managing the Sealib::Bitset used for Rank
     */
    explicit SharedRankStructure(std::shared_ptr<const Sealib::Bitset<uint8_t> > bitset);

    /**
     * default empty constructor
     */
    SharedRankStructure();

    /**
     * @return segment length
     */
    uint8_t getSegmentLength() const;

    /**
     * @return segment count
     */
    uint32_t getSegmentCount() const;

    /**
     * @return size of bitset
     */
    uint64_t size() const;

    /**
     * @return segment of the bitset
     */
    const Sealib::Bitset<uint8_t>& getBitset() const;

    ~SharedRankStructure();
    uint32_t setBefore(uint64_t segment) const;
};
}  // namespace Sealib
#endif  // SEALIB_SHAREDRANKSTRUCTURE_H_
