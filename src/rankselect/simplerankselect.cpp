#include "sealib/dictionary/simplerankselect.h"

uint64_t Sealib::SimpleRankSelect::select(uint64_t k) const {
    if (k == 0 || k > selects.size()) return (uint64_t) -1;
    return selects[k - 1];
}

uint64_t Sealib::SimpleRankSelect::rank(uint64_t k) const {
    if (k == 0 || k > ranks.size()) return (uint64_t) -1;
    return ranks[k - 1];
}

Sealib::SimpleRankSelect::SimpleRankSelect(
    std::shared_ptr<const Sealib::Bitset<uint8_t>> bitset_) :
    bitset(std::move(bitset_)),
    ranks(bitset->size()),
    selects(bitset->size(), (uint64_t) -1) {
    uint32_t rank = 0;
    for (uint32_t i = 0; i < bitset->size(); i++) {
        if ((*bitset)[i]) {
            selects[rank++] = i + 1;
        }
        ranks[i] = rank;
    }
    selects.resize(rank);
}
