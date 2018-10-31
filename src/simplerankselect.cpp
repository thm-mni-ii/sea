#include "sealib/simplerankselect.h"

unsigned long Sealib::SimpleRankSelect::select(unsigned long k) const {
    if (k == 0 || k > selects.size()) return (unsigned long) -1;
    return selects[k - 1];
}

unsigned long Sealib::SimpleRankSelect::rank(unsigned long k) const {
    if (k == 0 || k > ranks.size()) return (unsigned long) -1;
    return ranks[k - 1];
}

Sealib::SimpleRankSelect::SimpleRankSelect(
    std::shared_ptr<const Sealib::Bitset<unsigned char>> bitset_) :
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

Sealib::SimpleRankSelect::SimpleRankSelect() = default;

Sealib::SimpleRankSelect::~SimpleRankSelect() = default;
