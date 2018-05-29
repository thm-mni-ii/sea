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
    static const unsigned char segmentLength = 8;
    const boost::dynamic_bitset<unsigned char> bitset;
    unsigned int segmentCount;
    unsigned int maxRank;


    std::vector<unsigned int> setCountTable;
    std::vector<unsigned int> nonEmptySegments;

    unsigned int setBefore(unsigned long segment) const;


 public:

    static unsigned char log2(unsigned long bits);
    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    unsigned long rank(unsigned long k) const;

    /**
     * @param bitset rankselect bitset
     */
    explicit RankStructure(const boost::dynamic_bitset<unsigned char> &bitset_);

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
