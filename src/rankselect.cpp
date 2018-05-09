//
// Created by jmeintrup on 03.05.18.
//

#include "sealib/rankselect.h"

unsigned long Sealib::RankSelect::select(unsigned int k) const {
    unsigned long i = bitset.find_first();
    unsigned long cnt = 1;
    if (cnt == k) return i;
    while ((i = bitset.find_next(i)) != boost::dynamic_bitset<>::npos) {
        if (++cnt == k) break;
    }
    return i;
}

unsigned long Sealib::RankSelect::rank(unsigned int k) const {
    unsigned long rank = 0;
    for (unsigned long i = 0; i <= k; i++) {
        if (bitset[i]) rank++;
    }
    return rank;
}

Sealib::RankSelect::RankSelect(const boost::dynamic_bitset<> &bitset_) : bitset(bitset_) {}

Sealib::RankSelect::RankSelect() {}
