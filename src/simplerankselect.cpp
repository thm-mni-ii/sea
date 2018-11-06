#include "sealib/simplerankselect.h"

template <typename BlockType>
unsigned long Sealib::SimpleRankSelect<BlockType>::select(unsigned long k) const {
    if (k == 0 || k > selects.size()) return (unsigned long) -1;
    return selects[k - 1];
}

template <typename BlockType>
unsigned long Sealib::SimpleRankSelect<BlockType>::rank(unsigned long k) const {
    if (k == 0 || k > ranks.size()) return (unsigned long) -1;
    return ranks[k - 1];
}

template <typename BlockType>
Sealib::SimpleRankSelect<BlockType>::SimpleRankSelect(
    std::shared_ptr<const Sealib::Bitset<BlockType>> bitset_) :
    bitset(std::move(bitset_)),
    ranks(bitset->size()),
    selects(bitset->size(), (unsigned long) -1) {
    unsigned int rank = 0;
    for (unsigned int i = 0; i < bitset->size(); i++) {
        if ((*bitset)[i]) {
            selects[rank++] = i + 1;
        }
        ranks[i] = rank;
    }
    selects.resize(rank);
}

template <typename BlockType>
Sealib::SimpleRankSelect<BlockType>::SimpleRankSelect() = default;

template <typename BlockType>
Sealib::SimpleRankSelect<BlockType>::~SimpleRankSelect() = default;

namespace Sealib {
template
class SimpleRankSelect<unsigned char>;
template
class SimpleRankSelect<unsigned short>;
}  // namespace Sealib
