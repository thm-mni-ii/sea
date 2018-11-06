#ifndef SEALIB_SHAREDRANKSTRUCTURE_H_
#define SEALIB_SHAREDRANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

#include <sealib/bitset.h>
#include <memory>
#include <vector>

namespace Sealib {

template <typename BlockType = unsigned char>
class SharedRankStructure;
}

/**
 * Space efficient RankStructure implementation.
 * Uses a shared_ptr for the bitset, saves space if the bitset can be reused.
 * @author Johannes Meintrup
 */
namespace Sealib {
template <typename BlockType>
class SharedRankStructure {
 protected:
    static const unsigned char segmentLength = sizeof(BlockType) * 8;
    std::shared_ptr<const Sealib::Bitset<BlockType>> bitset;
    unsigned int segmentCount;
    unsigned int maxRank;

    std::vector<unsigned int> setCountTable;
    std::vector<unsigned int> nonEmptySegments;

 public:
    unsigned int getMaxRank() const;
    const std::vector<unsigned int> &getSetCountTable() const;
    const std::vector<unsigned int> &getNonEmptySegments() const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    unsigned long rank(unsigned long k) const;

    /**
     * @param shared_ptr managing the Sealib::Bitset used for Rank
     */
    explicit SharedRankStructure(std::shared_ptr<const Sealib::Bitset<BlockType> > bitset);

    /**
     * default empty constructor
     */
    SharedRankStructure();

    /**
     * @return segment length
     */
    unsigned char getSegmentLength() const;

    /**
     * @return segment count
     */
    unsigned int getSegmentCount() const;

    /**
     * @return size of bitset
     */
    unsigned long size() const;

    /**
     * @return segment of the bitset
     */
    const Sealib::Bitset<BlockType>& getBitset() const;

    ~SharedRankStructure();
    unsigned int setBefore(unsigned long segment) const;
};
}  // namespace Sealib
#endif  // SEALIB_SHAREDRANKSTRUCTURE_H_
