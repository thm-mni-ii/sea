#include "sealib/_types.h"
#include "sealib/collection/staticspacestorage.h"
#include <cmath>
#include <numeric>
#define PRELUDE                                                        \
    uint64_t start = rankSelect.select(i + 1) - i - 1;                 \
    uint64_t end = start + getSize(i) - 1;                             \
    uint64_t startBlock = start / bitsize, startBit = start % bitsize, \
             endBlock = end / bitsize, endBit = end % bitsize;         \
    uint64_t endGap = bitsize - endBit - 1;

#define IF_SINGLE_BLOCK           \
    if (startBlock == endBlock) { \
        uint64_t mask = (one << (endBit - startBit + 1)) - 1;

#define ELSE_IF_NOT_SINGLE_BLOCK                                \
    }                                                           \
    else { /*NOLINT*/                                           \
        uint64_t startMask = (one << (bitsize - startBit)) - 1, \
                 endMask = (one << (endBit + 1)) - 1;

#define END }

using Sealib::uint;
using Sealib::Bitset;
using Sealib::RankSelect;
using Sealib::StaticSpaceStorage;

uint64_t StaticSpaceStorage::get(uint i) const {
    PRELUDE
    uint64_t r = 0;
    IF_SINGLE_BLOCK
    r = (storage[startBlock] >> endGap) & mask;
    ELSE_IF_NOT_SINGLE_BLOCK
    r = (storage[startBlock] & startMask) << (endBit + 1);
    r |= (storage[endBlock] & (endMask << endGap)) >> endGap;
    END return r;
}

void StaticSpaceStorage::insert(uint i, uint64_t v) {
    PRELUDE
    IF_SINGLE_BLOCK
    storage[startBlock] &= ~(mask << endGap);
    storage[startBlock] |= v << endGap;
    ELSE_IF_NOT_SINGLE_BLOCK
    storage[startBlock] &= ~startMask;
    storage[startBlock] |= (v >> (endBit + 1));
    storage[endBlock] &= ~(endMask << endGap);
    storage[endBlock] |= (v & endMask) << endGap;
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

static void checkSize(const std::vector<bool> *v, uint maxBitsize) {
    uint bits = 0;
    for (bool b : *v) {
        if (b == 1)
            bits = 0;
        else
            bits++;
        assert(bits < maxBitsize);
    }
}

StaticSpaceStorage::StaticSpaceStorage(const std::vector<bool> &bits)
    : n(countWhere(&bits, 1)),
      pattern(bits),
      rankSelect(pattern),
      storage(countWhere(&bits, 0)) {
    checkSize(&bits, bitsize);
}

static std::vector<bool> makeBits(Sealib::Graph *g) {
    std::vector<bool> bits;
    for (uint u = 0; u < g->getOrder(); u++) {
        bits.push_back(1);
        for (uint k = 0; k < ceil(log2(g->getNodeDegree(u) + 1)); k++) {
            bits.push_back(0);
        }
    }
    return bits;
}

StaticSpaceStorage::StaticSpaceStorage(Graph *g)
    : StaticSpaceStorage(makeBits(g)) {}
