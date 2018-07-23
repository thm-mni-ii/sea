#ifndef SEALIB_SHAREDRANKSTRUCT_H
#define SEALIB_SHAREDRANKSTRUCT_H
#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)


#include <sealib/rankstructure.h>
#include <sealib/bitset.h>
#include <memory>

/**
 * Space efficient RankStructure implementation.
 * @author Johannes Meintrup
 */
namespace Sealib{
class SharedRankStructure : public RankStructure{
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
    unsigned long rank(unsigned long k) const;

    /**
     * @param bitset rankselect bitset
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
};
}  // namespace Sealib
#endif  // SEALIB_RANKSTRUCT_H
