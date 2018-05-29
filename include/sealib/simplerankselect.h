//
// Created by jmeintrup on 10.05.18.
//

#ifndef SEA_SIMPLERANKSELECT_H
#define SEA_SIMPLERANKSELECT_H
#include <boost/dynamic_bitset.hpp>
namespace Sealib {
class SimpleRankSelect {
 private:
    boost::dynamic_bitset<unsigned char> bitset;

 public:
    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    unsigned long select(unsigned long k) const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    unsigned long rank(unsigned long k) const;

    SimpleRankSelect(const boost::dynamic_bitset<unsigned char> &bitset);
    SimpleRankSelect();
};
}
#endif //SEA_SIMPLERANKSELECT_H
