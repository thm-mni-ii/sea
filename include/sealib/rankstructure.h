#ifndef SEALIB_RANKSTRUCTURE_H_
#define SEALIB_RANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

#include <sealib/bitset.h>
#include <memory>
#include <vector>

namespace Sealib {

template <typename BlockType = unsigned char>
class RankStructure;
}

/**
 * Space efficient RankStructure implementation.
 * @author Johannes Meintrup
 */
namespace Sealib {
template <typename BlockType>
class RankStructure {
 protected:
    static const unsigned char segmentLength = sizeof(BlockType) * 8;
    const Sealib::Bitset<BlockType> bitset;
    unsigned int segmentCount;
    unsigned int maxRank;

    std::vector<unsigned int> setCountTable;
    std::vector<unsigned int> nonEmptySegments;

 public:
    unsigned long size() const;
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
     * @param bitset used for Rank
     */
    explicit RankStructure(const Sealib::Bitset<BlockType> &bitset);

    /**
     * default empty constructor
     */
    RankStructure();

    /**
     * @return segment length
     */
    unsigned char getSegmentLength() const;

    /**
     * @return segment count
     */
    unsigned int getSegmentCount() const;

    /**
     * @return segment of the bitset
     */
    const Sealib::Bitset<BlockType>& getBitset() const;

    ~RankStructure();
    unsigned int setBefore(unsigned long segment) const;
};
}  // namespace Sealib
#endif  // SEALIB_RANKSTRUCTURE_H_
