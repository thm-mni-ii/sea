#ifndef SEALIB_SIMPLERANKSELECT_H_
#define SEALIB_SIMPLERANKSELECT_H_

#include <sealib/bitset.h>
#include <memory>
#include <vector>

namespace Sealib {

template <typename BlockType = unsigned char>
class SimpleRankSelect;
}

namespace Sealib {
/**
 * Naive implementation of RankSelect, used to test the space efficient variant or for debugging
 * @author Johannes Meintrup
 */
template <typename BlockType>
class SimpleRankSelect {
 private:
    std::shared_ptr<const Bitset<BlockType>> bitset;
    std::vector<unsigned long> ranks;
    std::vector<unsigned long> selects;

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

    explicit SimpleRankSelect(std::shared_ptr<const Bitset<BlockType>> bitset);
    SimpleRankSelect();
    ~SimpleRankSelect();
};
}  // namespace Sealib
#endif  // SEALIB_SIMPLERANKSELECT_H_
