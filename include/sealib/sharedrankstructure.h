#ifndef SEALIB_SHAREDRANKSTRUCTURE_H_
#define SEALIB_SHAREDRANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

#include <sealib/rankstructure.h>
#include <sealib/bitset.h>
#include <memory>
#include <vector>

/**
 * Space efficient RankStructure implementation.
 * Uses a shared_ptr for the bitset, saves space if the bitset can be reused.
 * @author Johannes Meintrup
 */
namespace Sealib {
class SharedRankStructure : public RankStructure {
 protected:
    static const unsigned char segmentLength = 8;
    std::shared_ptr<Sealib::Bitset<unsigned char>> bitset;
    unsigned int segmentCount;
    unsigned int maxRank;

    std::vector<unsigned int> setCountTable;
    std::vector<unsigned int> nonEmptySegments;

    unsigned int setBefore(unsigned long segment) const;

 public:
    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    unsigned long rank(unsigned long k) const override;

    /**
     * @param shared_ptr managing the Sealib::Bitset used for Rank
     */
    explicit SharedRankStructure(std::shared_ptr<Sealib::Bitset<unsigned char> > bitset);

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

    virtual ~SharedRankStructure();
};
}  // namespace Sealib
#endif  // SEALIB_SHAREDRANKSTRUCTURE_H_
