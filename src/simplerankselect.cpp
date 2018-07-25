#include "sealib/simplerankselect.h"

unsigned long Sealib::SimpleRankSelect::select(unsigned long k) const {
    if (k == 0)
        return Bitset<>::npos;

    unsigned long i = Bitset<>::npos;
    for (unsigned long j = 0; j < bitset->size(); j++) {
        if ((*bitset)[j]) {
            i = j;
            break;
        }
    }
    if (i == Bitset<>::npos)
        return i;

    unsigned long cnt = 1;
    if (cnt == k)
        return i + 1;
    for (unsigned long j = i + 1; j < bitset->size(); j++) {
        if ((*bitset)[j] && (++cnt == k)) {
            break;
        }
    }

    return cnt != k ? Bitset<>::npos : i + 1;
}

unsigned long Sealib::SimpleRankSelect::rank(unsigned long k) const {
    if (k == 0) {
        return Bitset<>::npos;
    }
    if (bitset->size() < k) {
        return Bitset<>::npos;
    }
    unsigned long rank = 0;
    for (unsigned long i = 0; i < k; i++) {
        if ((*bitset)[i]) rank++;
    }
    return rank;
}

Sealib::SimpleRankSelect::SimpleRankSelect(std::shared_ptr<Sealib::Bitset<unsigned char>> bitset_) :
    bitset(std::move(bitset_)) {}

Sealib::SimpleRankSelect::SimpleRankSelect() = default;

Sealib::SimpleRankSelect::~SimpleRankSelect() = default;
