#ifndef SEALIB_SIMPLERANKSELECT_H_
#define SEALIB_SIMPLERANKSELECT_H_

#include <sealib/bitset.h>
#include <sealib/rankselect.h>
#include <memory>

namespace Sealib {
/**
 * Naive implementation of RankSelect, used to test the space efficient variant or for debugging
 * @author Johannes Meintrup
 */
class SimpleRankSelect : public RankSelect {
 private:
    std::shared_ptr<Bitset<unsigned char>> bitset;

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

    explicit SimpleRankSelect(std::shared_ptr<Bitset<unsigned char>> bitset);
    SimpleRankSelect();
    ~SimpleRankSelect() override;
};
}  // namespace Sealib
#endif  // SEALIB_SIMPLERANKSELECT_H_
