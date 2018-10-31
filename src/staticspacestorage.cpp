#include "sealib/staticspacestorage.h"
#include <cstdio>
#include <numeric>

using Sealib::Bitset;
using Sealib::RankSelect;
using Sealib::StaticSpaceStorage;

StaticSpaceStorage::Word StaticSpaceStorage::get(uint i) const {
    unsigned long start = rankSelect.select(i + 1) - i - 1;
    unsigned long end = start + getSize(i) - 1;
    unsigned long startBlock = start / sizeof(Word), startBit = start % bitsize,
                  endBlock = end / sizeof(Word), endBit = end % bitsize;
    Word r = 0;
    if (startBlock == endBlock) {
        Word a = storage[startBlock];
        a &= ((1 << (endBit - startBit + 1)) - 1) << (bitsize - endBit - 1);
        a >>= (bitsize - endBit - 1);
        return a;
    } else {
        for (unsigned long b = startBlock; b <= endBlock; b++) {
            if (b == startBlock) {
                Word a = storage[b];
                a &= ((1 << (bitsize - startBit + 1)) - 1);
                r = a;
            } else if (b == endBlock) {
                Word a = storage[b];
                a &= ((1 << endBit) - 1) << (bitsize - endBit - 1);
            } else {
                // can never happen?
                r = (r << bitsize) | storage[b];
            }
        }
    }
    /*bool first = true;
    for (unsigned long a = end;
         static_cast<long>(a) >= static_cast<long>(start); a--) {
        if (!first) {
            r <<= 1;
        } else {
            first = false;
        }
        r |= storage[a];
    }*/
    return r;
}

void StaticSpaceStorage::insert(uint i, StaticSpaceStorage::Word v) {
    unsigned long start = rankSelect.select(i + 1) - i - 1;
    unsigned long end = start + getSize(i) - 1;
    unsigned long startBlock = start / sizeof(Word), startBit = start % bitsize,
                  endBlock = end / sizeof(Word), endBit = end % bitsize;
    uint r = 0;
    if (startBlock == endBlock) {
        Word a = v;
        a <<= bitsize - endBit - 1;
        storage[startBlock] |= a;
    } else {
        Word a = v;
        storage[startBlock] &= (1 << (startBit - 1))
                               << (bitsize - startBit + 1);
        storage[startBlock] |= (v >> endBit);
        storage[endBlock] &= (1 << (endBit - 1)) << (bitsize - endBit - 1);
        storage[endBlock] |= (v & ((1 << endBit) - 1))
                             << (bitsize - endBit - 1);
    }
    /*for (unsigned long a = pos + getSize(i) - 1;
         static_cast<long>(a) >= static_cast<long>(pos); a--) {
        storage[a] = v & 1;
        v >>= 1;
    }*/
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
