#ifndef SEALIB_SIMPLERANKSELECT_H
#define SEALIB_SIMPLERANKSELECT_H

#include <sealib/bitset.h>
#include <sealib/rankselect.h>

namespace Sealib {
/**
 * Naive implementation of RankSelect, used to test the space efficient variant or for debugging
 * @author Johannes Meintrup
 */
class SimpleRankSelect : public RankSelect{
 private:
    Bitset<unsigned char> bitset;

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

    SimpleRankSelect(Bitset<unsigned char> bitset);
    SimpleRankSelect();
};
}
#endif //SEA_SIMPLERANKSELECT_H
