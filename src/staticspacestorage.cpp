#include "sealib/staticspacestorage.h"
#include <cstdio>
#include <numeric>

using Sealib::Bitset;
using Sealib::RankSelect;
using Sealib::StaticSpaceStorage;

uint StaticSpaceStorage::get(uint i) const {
    unsigned long pos = rankSelect.select(i + 1) - i - 1;
    unsigned long size = getEnd(i + 1) - rankSelect.select(i + 1) - 1;
    uint r = 0;
    bool first = true;
    for (unsigned long a = pos + size - 1;
         static_cast<long>(a) >= static_cast<long>(pos); a--) {
        if (!first) {
            r <<= 1;
        } else {
            first = false;
        }
        r |= storage[a];
    }
    return r;
}

void StaticSpaceStorage::insert(uint i, uint v) {
    unsigned long pos = rankSelect.select(i + 1) - i - 1;
    unsigned long size = getEnd(i + 1) - rankSelect.select(i + 1) - 1;
    for (unsigned long a = pos + size - 1;
         static_cast<long>(a) >= static_cast<long>(pos); a--) {
        storage[a] = v & 1;
        v >>= 1;
    }
}

std::vector<bool> StaticSpaceStorage::makeBitVector(std::vector<uint> *sizes) {
    std::vector<bool> r(sizes->size() +
                        std::accumulate(sizes->begin(), sizes->end(), 0UL));
    uint index = 0;
    for (uint a : *sizes) {
        r[index] = 1;
        for (uint b = 0; b < a; b++) {
            index++;
        }
        index++;
    }
    return r;
}

static Bitset<uint_fast8_t> initPattern(const std::vector<bool> *v) {
    Bitset<uint_fast8_t> r(v->size());
    uint index = 0;
    for (bool a : *v) {
        r[index++] = a;
    }
    return r;
}

static uint countWhere(const std::vector<bool> *v, bool x) {
    uint r = 0;
    for (bool a : *v) {
        if (a == x) r++;
    }
    return r;
}

StaticSpaceStorage::StaticSpaceStorage(const std::vector<bool> &bits)
    : n(countWhere(&bits, 1)),
      pattern(initPattern(&bits)),
      rankSelect(pattern),
      storage(countWhere(&bits, 0)) {}
