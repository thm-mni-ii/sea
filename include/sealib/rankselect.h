//
// Created by jmeintrup on 03.05.18.
//

#ifndef SEA_RANKSELECT_H
#define SEA_RANKSELECT_H

#include <boost/dynamic_bitset.hpp>

namespace Sealib{
/**
 * Space efficient RankSelect implementation.
 * @author Johannes Meintrup
 */
class RankSelect {
    boost::dynamic_bitset<> bitset;

 public:
    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    unsigned long select(unsigned int k) const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    unsigned long rank(unsigned int k) const;

    /**
     * @param bitset rankselect bitset
     */
    explicit RankSelect(const boost::dynamic_bitset<> &bitset);

    /**
     * default empty constructor
     */
    RankSelect();
};
}  // namespace Sealib
#endif  // SEA_RANKSELECT_H
