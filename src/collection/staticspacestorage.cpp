#include "sealib/collection/staticspacestorage.h"
#include <cmath>
#include <numeric>
#include "sealib/_types.h"
#define PRELUDE                                                            \
    uint64_t start = rankSelect.select(i + 1) - i - 1;                     \
    uint64_t size = getSize(i);                                            \
    uint64_t end = size == 0 ? 0 : start + size - 1;                       \
    uint64_t startBlock = start / WORD_SIZE, startBit = start % WORD_SIZE, \
             endBlock = end / WORD_SIZE, endBit = end % WORD_SIZE;         \
    uint64_t endGap = WORD_SIZE - endBit - 1;

#define IF_SINGLE_BLOCK                       \
    if (startBlock == endBlock && size > 0) { \
        uint64_t mask = (one << (endBit - startBit + 1)) - 1;

#define ELSE_IF_NOT_SINGLE_BLOCK                                  \
    }                                                             \
    else if (size > 0) { /*NOLINT*/                               \
        uint64_t startMask = (one << (WORD_SIZE - startBit)) - 1, \
                 endMask = (one << (endBit + 1)) - 1;

#define END }

namespace Sealib {

uint64_t StaticSpaceStorage::get(uint64_t i) const {
    PRELUDE
    uint64_t r = 0;
    IF_SINGLE_BLOCK
    r = (storage[startBlock] >> endGap) & mask;
    ELSE_IF_NOT_SINGLE_BLOCK
    r = (storage[startBlock] & startMask) << (endBit + 1);
    r |= (storage[endBlock] & (endMask << endGap)) >> endGap;
    END return r;
}

void StaticSpaceStorage::insert(uint64_t i, uint64_t v) {
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

std::vector<bool> StaticSpaceStorage::makeBitVector(
    std::vector<uint64_t> const &sizes) {
    std::vector<bool> r(sizes.size() +
                        std::accumulate(sizes.begin(), sizes.end(), 0UL));
    uint64_t index = 0;
    for (uint64_t a : sizes) {
        r[index] = 1;
        for (uint64_t b = 0; b < a; b++) {
            index++;
        }
        index++;
    }
    return r;
}

static inline uint64_t countSetBits(const std::vector<bool> *v) {
    uint64_t r = 0;
    for (bool a : *v) {
        if (a) r++;
    }
    return r;
}

StaticSpaceStorage::StaticSpaceStorage(const std::vector<bool> &bits)
    : n(countSetBits(&bits)),
      pattern(bits),
      rankSelect(pattern),
      storage((bits.size() - n) / WORD_SIZE + 1) {}

static std::vector<bool> makeBits(Sealib::Graph const &g) {
    std::vector<bool> bits;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        bits.push_back(1);
        for (uint64_t k = 0; k < ceil(log2(g.deg(u) + 1)); k++) {
            bits.push_back(0);
        }
    }
    return bits;
}

StaticSpaceStorage::StaticSpaceStorage(Graph const &g)
    : StaticSpaceStorage(makeBits(g)) {}

}  // namespace Sealib
