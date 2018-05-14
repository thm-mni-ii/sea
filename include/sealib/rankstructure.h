//
// Created by jmeintrup on 03.05.18.
//

#ifndef SEA_RANKSTRUCT_H
#define SEA_RANKSTRUCT_H
#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

#include <sealib/dynamicbitsetextension.h>

/**
 * Space efficient RankStructure implementation.
 * @author Johannes Meintrup
 */
namespace Sealib{
class RankStructure {
 protected:
    unsigned char segmentLength;
    unsigned int segmentCount;
    std::vector<std::vector<unsigned char>> localRankLookupTable;

    std::vector<unsigned int> setCountTable;
    std::vector<unsigned int> nonEmptySegments;

    unsigned char rankLocal(unsigned long segment, unsigned char localIdx) const;

    unsigned int setBefore(unsigned long segment) const;

    static unsigned char calculateSegmentLength(unsigned long bits);
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
    explicit RankStructure(const boost::dynamic_bitset<> &bitset);

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
};
}  // namespace Sealib
#endif  // SEA_RANKSTRUCT_H
