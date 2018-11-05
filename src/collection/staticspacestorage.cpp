#include "sealib/collection/staticspacestorage.h"
#include <numeric>
#define PRELUDE                                                             \
    uint64_t start = rankSelect.select(i + 1) - i - 1;                 \
    uint64_t end = start + getSize(i) - 1;                             \
    uint64_t startBlock = start / bitsize, startBit = start % bitsize, \
                  endBlock = end / bitsize, endBit = end % bitsize;

#define IF_SINGLE_BLOCK           \
    if (startBlock == endBlock) { \
        uint64_t mask = (1 << (endBit - startBit + 1)) - 1;

#define ELSE_IF_NOT_SINGLE_BLOCK                                       \
    }                                                                  \
    else { /*NOLINT*/                                                  \
        uint64_t startMask = (1 << (bitsize - startBit - 1)) - 1, \
                      endMask = (1 << endBit) - 1;

#define END }

using Sealib::Bitset;
using Sealib::RankSelect;
using Sealib::StaticSpaceStorage;

uint64_t StaticSpaceStorage::get(uint i) const {
    PRELUDE
    uint64_t r = 0;
    IF_SINGLE_BLOCK
    r = (storage[startBlock] >> (bitsize - endBit - 1)) & mask;
    ELSE_IF_NOT_SINGLE_BLOCK
    r = (storage[startBlock] & startMask) << endBit;
    r |= storage[endBlock] & (endMask << (bitsize - endBit - 1));
    r >>= (bitsize - endBit - 1);
    END return r;
}

void StaticSpaceStorage::insert(uint i, uint64_t v) {
    PRELUDE

    IF_SINGLE_BLOCK
    storage[startBlock] &= ~(mask << (bitsize - endBit - 1));
    storage[startBlock] |= v << (bitsize - endBit - 1);
    ELSE_IF_NOT_SINGLE_BLOCK
    storage[startBlock] &= ~startMask;
    storage[startBlock] |= (v >> endBit);
    storage[endBlock] &= endMask << (bitsize - endBit - 1);
    storage[endBlock] |= (v & endMask) << (bitsize - endBit - 1);
    END
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

static inline uint countWhere(const std::vector<bool> *v, bool x) {
    uint r = 0;
    for (bool a : *v) {
        if (a == x) r++;
    }
    return r;
}

StaticSpaceStorage::StaticSpaceStorage(const std::vector<bool> &bits)
    : n(countWhere(&bits, 1)),
      pattern(bits),
      rankSelect(pattern),
      storage(countWhere(&bits, 0)) {}
