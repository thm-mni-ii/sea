#ifndef SEALIB_DICTIONARY_RANK9SELECT_HPP_
#define SEALIB_DICTIONARY_RANK9SELECT_HPP_

#include <sealib/collection/bitset.h>

#include <cstdint>
#include <vector>

#include "rank9.hpp"

#define ONES_STEP_9                                              \
    (UINT64_C(1) << 0 | UINT64_C(1) << 9 | UINT64_C(1) << 18 |   \
     UINT64_C(1) << 27 | UINT64_C(1) << 36 | UINT64_C(1) << 45 | \
     UINT64_C(1) << 54)

#define ONES_STEP_16                                            \
    (UINT64_C(1) << 0 | UINT64_C(1) << 16 | UINT64_C(1) << 32 | \
     UINT64_C(1) << 48)

#define ULEQ_STEP_9(x, y)                                                     \
    (((((((y) | MSBS_STEP_9) - ((x) & ~MSBS_STEP_9)) | (x ^ y)) ^ (x & ~y)) & \
      MSBS_STEP_9) >>                                                         \
     8)

#define ULEQ_STEP_16(x, y)                                          \
    (((((((y) | MSBS_STEP_16) - ((x) & ~MSBS_STEP_16)) | (x ^ y)) ^ \
       (x & ~y)) &                                                  \
      MSBS_STEP_16) >>                                              \
     15)

#define MSBS_STEP_9 (UINT64_C(0x100) * ONES_STEP_9)
#define MSBS_STEP_16 (UINT64_C(0x8000) * ONES_STEP_16)

// Required by select64
constexpr uint8_t kSelectInByte[2048] = {
    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3,
    0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0,
    1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1,
    0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0,
    2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2,
    0, 1, 0, 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0,
    1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1,
    0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0,
    3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0,
    1, 0, 2, 0, 1, 0, 8, 8, 8, 1, 8, 2, 2, 1, 8, 3, 3, 1, 3, 2, 2, 1, 8, 4, 4,
    1, 4, 2, 2, 1, 4, 3, 3, 1, 3, 2, 2, 1, 8, 5, 5, 1, 5, 2, 2, 1, 5, 3, 3, 1,
    3, 2, 2, 1, 5, 4, 4, 1, 4, 2, 2, 1, 4, 3, 3, 1, 3, 2, 2, 1, 8, 6, 6, 1, 6,
    2, 2, 1, 6, 3, 3, 1, 3, 2, 2, 1, 6, 4, 4, 1, 4, 2, 2, 1, 4, 3, 3, 1, 3, 2,
    2, 1, 6, 5, 5, 1, 5, 2, 2, 1, 5, 3, 3, 1, 3, 2, 2, 1, 5, 4, 4, 1, 4, 2, 2,
    1, 4, 3, 3, 1, 3, 2, 2, 1, 8, 7, 7, 1, 7, 2, 2, 1, 7, 3, 3, 1, 3, 2, 2, 1,
    7, 4, 4, 1, 4, 2, 2, 1, 4, 3, 3, 1, 3, 2, 2, 1, 7, 5, 5, 1, 5, 2, 2, 1, 5,
    3, 3, 1, 3, 2, 2, 1, 5, 4, 4, 1, 4, 2, 2, 1, 4, 3, 3, 1, 3, 2, 2, 1, 7, 6,
    6, 1, 6, 2, 2, 1, 6, 3, 3, 1, 3, 2, 2, 1, 6, 4, 4, 1, 4, 2, 2, 1, 4, 3, 3,
    1, 3, 2, 2, 1, 6, 5, 5, 1, 5, 2, 2, 1, 5, 3, 3, 1, 3, 2, 2, 1, 5, 4, 4, 1,
    4, 2, 2, 1, 4, 3, 3, 1, 3, 2, 2, 1, 8, 8, 8, 8, 8, 8, 8, 2, 8, 8, 8, 3, 8,
    3, 3, 2, 8, 8, 8, 4, 8, 4, 4, 2, 8, 4, 4, 3, 4, 3, 3, 2, 8, 8, 8, 5, 8, 5,
    5, 2, 8, 5, 5, 3, 5, 3, 3, 2, 8, 5, 5, 4, 5, 4, 4, 2, 5, 4, 4, 3, 4, 3, 3,
    2, 8, 8, 8, 6, 8, 6, 6, 2, 8, 6, 6, 3, 6, 3, 3, 2, 8, 6, 6, 4, 6, 4, 4, 2,
    6, 4, 4, 3, 4, 3, 3, 2, 8, 6, 6, 5, 6, 5, 5, 2, 6, 5, 5, 3, 5, 3, 3, 2, 6,
    5, 5, 4, 5, 4, 4, 2, 5, 4, 4, 3, 4, 3, 3, 2, 8, 8, 8, 7, 8, 7, 7, 2, 8, 7,
    7, 3, 7, 3, 3, 2, 8, 7, 7, 4, 7, 4, 4, 2, 7, 4, 4, 3, 4, 3, 3, 2, 8, 7, 7,
    5, 7, 5, 5, 2, 7, 5, 5, 3, 5, 3, 3, 2, 7, 5, 5, 4, 5, 4, 4, 2, 5, 4, 4, 3,
    4, 3, 3, 2, 8, 7, 7, 6, 7, 6, 6, 2, 7, 6, 6, 3, 6, 3, 3, 2, 7, 6, 6, 4, 6,
    4, 4, 2, 6, 4, 4, 3, 4, 3, 3, 2, 7, 6, 6, 5, 6, 5, 5, 2, 6, 5, 5, 3, 5, 3,
    3, 2, 6, 5, 5, 4, 5, 4, 4, 2, 5, 4, 4, 3, 4, 3, 3, 2, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 4, 8, 4, 4, 3,
    8, 8, 8, 8, 8, 8, 8, 5, 8, 8, 8, 5, 8, 5, 5, 3, 8, 8, 8, 5, 8, 5, 5, 4, 8,
    5, 5, 4, 5, 4, 4, 3, 8, 8, 8, 8, 8, 8, 8, 6, 8, 8, 8, 6, 8, 6, 6, 3, 8, 8,
    8, 6, 8, 6, 6, 4, 8, 6, 6, 4, 6, 4, 4, 3, 8, 8, 8, 6, 8, 6, 6, 5, 8, 6, 6,
    5, 6, 5, 5, 3, 8, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, 8, 8, 8, 8,
    8, 8, 8, 7, 8, 8, 8, 7, 8, 7, 7, 3, 8, 8, 8, 7, 8, 7, 7, 4, 8, 7, 7, 4, 7,
    4, 4, 3, 8, 8, 8, 7, 8, 7, 7, 5, 8, 7, 7, 5, 7, 5, 5, 3, 8, 7, 7, 5, 7, 5,
    5, 4, 7, 5, 5, 4, 5, 4, 4, 3, 8, 8, 8, 7, 8, 7, 7, 6, 8, 7, 7, 6, 7, 6, 6,
    3, 8, 7, 7, 6, 7, 6, 6, 4, 7, 6, 6, 4, 6, 4, 4, 3, 8, 7, 7, 6, 7, 6, 6, 5,
    7, 6, 6, 5, 6, 5, 5, 3, 7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 8, 8, 8,
    8, 8, 8, 8, 5, 8, 8, 8, 5, 8, 5, 5, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 6, 8, 8, 8, 8, 8, 8, 8, 6, 8, 8, 8, 6, 8, 6, 6, 4, 8, 8, 8, 8, 8,
    8, 8, 6, 8, 8, 8, 6, 8, 6, 6, 5, 8, 8, 8, 6, 8, 6, 6, 5, 8, 6, 6, 5, 6, 5,
    5, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8,
    7, 8, 8, 8, 7, 8, 7, 7, 4, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 7, 8, 7, 7, 5,
    8, 8, 8, 7, 8, 7, 7, 5, 8, 7, 7, 5, 7, 5, 5, 4, 8, 8, 8, 8, 8, 8, 8, 7, 8,
    8, 8, 7, 8, 7, 7, 6, 8, 8, 8, 7, 8, 7, 7, 6, 8, 7, 7, 6, 7, 6, 6, 4, 8, 8,
    8, 7, 8, 7, 7, 6, 8, 7, 7, 6, 7, 6, 6, 5, 8, 7, 7, 6, 7, 6, 6, 5, 7, 6, 6,
    5, 6, 5, 5, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 8, 8, 8, 8, 8, 8, 6,
    8, 8, 8, 6, 8, 6, 6, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 7, 8, 7, 7, 5, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 7,
    8, 7, 7, 6, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 7, 8, 7, 7, 6, 8, 8, 8, 7, 8,
    7, 7, 6, 8, 7, 7, 6, 7, 6, 6, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    7, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 7, 8, 7, 7, 6, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7};

/** Returns the index of the k-th 1-bit in the 64-bit word x.
 * @param x 64-bit word.
 * @param k 0-based rank (`k = 0` returns the position of the first 1-bit).
 *
 * Uses the broadword selection algorithm by Vigna [1], improved by Gog and
 * Petri [2] and Vigna [3]. Facebook's Folly implementation [4].
 *
 * [1] Sebastiano Vigna. Broadword Implementation of Rank/Select Queries.
 * WEA, 2008
 *
 * [2] Simon Gog, Matthias Petri. Optimized succinct data structures for
 * massive data. Softw. Pract. Exper., 2014
 *
 * [3] Sebastiano Vigna. MG4J 5.2.1. http://mg4j.di.unimi.it/
 *
 * [4] Facebook Folly library: https://github.com/facebook/folly
 *
 */
inline uint64_t select64(uint64_t x, uint64_t k) {
#ifndef __haswell__
    constexpr uint64_t kOnesStep4 = 0x1111111111111111ULL;
    constexpr uint64_t kOnesStep8 = 0x0101010101010101ULL;
    constexpr uint64_t kLAMBDAsStep8 = 0x80ULL * kOnesStep8;

    auto s = x;
    s = s - ((s & 0xA * kOnesStep4) >> 1);
    s = (s & 0x3 * kOnesStep4) + ((s >> 2) & 0x3 * kOnesStep4);
    s = (s + (s >> 4)) & 0xF * kOnesStep8;
    uint64_t byteSums = s * kOnesStep8;

    uint64_t kStep8 = k * kOnesStep8;
    uint64_t geqKStep8 =
        (((kStep8 | kLAMBDAsStep8) - byteSums) & kLAMBDAsStep8);
    uint64_t place = __builtin_popcountll(geqKStep8) * 8;
    uint64_t byteRank = k - (((byteSums << 8) >> place) & uint64_t(0xFF));
    return place + kSelectInByte[((x >> place) & 0xFF) | (byteRank << 8)];
#elif defined(__GNUC__) || defined(__clang__)
    // GCC and Clang won't inline the intrinsics.
    uint64_t result = uint64_t(1) << k;

    asm("pdep %1, %0, %0\n\t"
        "tzcnt %0, %0"
        : "+r"(result)
        : "r"(x));

    return result;
#else
    return _tzcnt_u64(_pdep_u64(1ULL << k, x));
#endif
}

namespace Sealib {

/** A class implementing a combination of Rank9 for ranking and Select9 for
 * selection. Select9 uses 25%-37.5% additional space (beside the 25% due to
 * Rank9), depending on density, and provides constant-time selection.
 *
 * Reference implementation by Sebastiano Vigna in the sux library:
 * https://github.com/vigna/sux
 *
 * Related Paper:
 * Sebastiano Vigna. 2008. Broadword implementation of rank/select queries. In
 * Proceedings of the 7th international conference on Experimental algorithms
 * (WEA’08). Springer-Verlag, Berlin, Heidelberg, 154–168.
 */
class Rank9Select : public Rank9 {
 private:
    static const int log2_ones_per_inventory = 9;
    static const int ones_per_inventory = 1 << log2_ones_per_inventory;
    static const uint64_t inventory_mask = ones_per_inventory - 1;

    std::vector<uint64_t> inventory, subinventory;
    uint64_t inventory_size{};

    void init() {
        const uint64_t num_words = (this->m_bitset.size() + 63) / 64;
        inventory_size =
            (this->num_ones + ones_per_inventory - 1) / ones_per_inventory;

        assert(ones_per_inventory <= 8 * 64);

        inventory.resize(inventory_size + 1);
        subinventory.resize((num_words + 3) / 4);

        uint64_t d = 0;
        for (uint64_t i = 0; i < num_words; i++)
            for (int j = 0; j < 64; j++)
                if (this->m_bitset.data()[i] & 1ULL << j) {
                    if ((d & inventory_mask) == 0) {
                        inventory[d >> log2_ones_per_inventory] = i * 64 + j;
                        assert(this->counts[(i / 8) * 2] <= d);
                        assert(this->counts[(i / 8) * 2 + 2] > d);
                    }

                    d++;
                }

        assert(this->num_ones == d);
        inventory[inventory_size] = ((num_words + 3) & ~3ULL) * 64;

        d = 0;
        int state;
        uint64_t *s, first_bit, index, span, block_span, block_left,
            counts_at_start;

        for (uint64_t i = 0; i < num_words; i++)
            for (int j = 0; j < 64; j++)
                if (this->m_bitset.data()[i] & 1ULL << j) {
                    if ((d & inventory_mask) == 0) {
                        first_bit = i * 64 + j;
                        index = d >> log2_ones_per_inventory;
                        assert(inventory[index] == first_bit);
                        s = &subinventory[(inventory[index] / 64) / 4];
                        span = (inventory[index + 1] / 64) / 4 -
                               (inventory[index] / 64) / 4;
                        state = -1;
                        counts_at_start =
                            this->counts[((inventory[index] / 64) / 8) * 2];
                        block_span = (inventory[index + 1] / 64) / 8 -
                                     (inventory[index] / 64) / 8;
                        block_left = (inventory[index] / 64) / 8;

                        if (span >= 512) {
                            state = 0;
                        } else if (span >= 256) {
                            state = 1;
                        } else if (span >= 128) {
                            state = 2;

                        } else if (span >= 16) {
                            assert(((block_span + 8) & -8LL) + 8 <= span * 4);

                            uint64_t k;
                            for (k = 0; k < block_span; k++) {
                                assert(((uint16_t *)s)[k + 8] == 0);
                                ((uint16_t *)s)[k + 8] =
                                    this->counts[(block_left + k + 1) * 2] -
                                    counts_at_start;
                            }

                            for (; k < ((block_span + 8) & -8LL); k++) {
                                assert(((uint16_t *)s)[k + 8] == 0);
                                ((uint16_t *)s)[k + 8] = 0xFFFFU;
                            }

                            assert(block_span / 8 <= 8);

                            for (k = 0; k < block_span / 8; k++) {
                                assert(((uint16_t *)s)[k] == 0);
                                ((uint16_t *)s)[k] =
                                    this->counts[(block_left + (k + 1) * 8) *
                                                 2] -
                                    counts_at_start;
                            }

                            for (; k < 8; k++) {
                                assert(((uint16_t *)s)[k] == 0);
                                ((uint16_t *)s)[k] = 0xFFFFU;
                            }
                        } else if (span >= 2) {
                            assert(((block_span + 8) & -8LL) <= span * 4);

                            uint64_t k;
                            for (k = 0; k < block_span; k++) {
                                assert(((uint16_t *)s)[k] == 0);
                                ((uint16_t *)s)[k] =
                                    this->counts[(block_left + k + 1) * 2] -
                                    counts_at_start;
                            }

                            for (; k < ((block_span + 8) & -8LL); k++) {
                                assert(((uint16_t *)s)[k] == 0);
                                ((uint16_t *)s)[k] = 0xFFFFU;
                            }
                        }
                    }

                    switch (state) {
                        case 0:
                            assert(s[d & inventory_mask] == 0);
                            s[d & inventory_mask] = i * 64 + j;
                            break;
                        case 1:
                            assert(((uint32_t *)s)[d & inventory_mask] == 0);
                            assert(i * 64 + j - first_bit < (1ULL << 32));
                            ((uint32_t *)s)[d & inventory_mask] =
                                i * 64 + j - first_bit;
                            break;
                        case 2:
                            assert(((uint16_t *)s)[d & inventory_mask] == 0);
                            assert(i * 64 + j - first_bit < (1 << 16));
                            ((uint16_t *)s)[d & inventory_mask] =
                                i * 64 + j - first_bit;
                            break;
                    }

                    d++;
                }
    }

 public:
    /** Creates a new instance using a given bit set.
     *
     * **Warning**: if the bitset has bits set beyond it's own size()
     * the behaviour is undefined (aka everything explodes)
     *
     * @param bitset a Sealib::Bitset of 64-bit words.
     */
    explicit Rank9Select(Sealib::Bitset<uint64_t> bitset)
        : Rank9(std::move(bitset)),
          inventory(),
          subinventory(),
          inventory_size(0) {
        init();
    }

    /** Returns the position of the one with given rank.
     *
     * @param rank the desired rank (index) of a one in the bit vector.
     *
     * @return the position of the one with given rank;
     * panics if rank is out of bounds, i.e. larger than max rank
     */
    size_t select(uint64_t rank) {
        if (rank == 0 || rank > _max_rank) {
            return INVALID;
        }
        rank--;
        const uint64_t inventory_index_left = rank >> log2_ones_per_inventory;
        assert(inventory_index_left <= inventory_size);

        const uint64_t inventory_left = inventory[inventory_index_left];
        const uint64_t block_right = inventory[inventory_index_left + 1] / 64;
        uint64_t block_left = inventory_left / 64;
        const uint64_t span = block_right / 4 - block_left / 4;
        const uint64_t *const s = &subinventory[block_left / 4];
        uint64_t count_left, rank_in_block;

        if (span < 2) {
            block_left &= ~7;
            count_left = block_left / 4 & ~1;
            assert(rank < this->counts[count_left + 2]);
            rank_in_block = rank - this->counts[count_left];

        } else if (span < 16) {
            block_left &= ~7;
            count_left = block_left / 4 & ~1;
            const uint64_t rank_in_superblock = rank - this->counts[count_left];
            const uint64_t rank_in_superblock_step_16 =
                rank_in_superblock * ONES_STEP_16;

            const uint64_t first = s[0], second = s[1];
            const int where =
                (ULEQ_STEP_16(first, rank_in_superblock_step_16) +
                 ULEQ_STEP_16(second, rank_in_superblock_step_16)) *
                    ONES_STEP_16 >>
                47;
            assert(where >= 0);
            assert(where <= 16);

            block_left += where * 4;
            count_left += where;
            rank_in_block = rank - this->counts[count_left];
            assert(rank_in_block < 512);

        } else if (span < 128) {
            block_left &= ~7;
            count_left = block_left / 4 & ~1;
            const uint64_t rank_in_superblock = rank - this->counts[count_left];
            const uint64_t rank_in_superblock_step_16 =
                rank_in_superblock * ONES_STEP_16;

            const uint64_t first = s[0], second = s[1];
            const int where0 =
                (ULEQ_STEP_16(first, rank_in_superblock_step_16) +
                 ULEQ_STEP_16(second, rank_in_superblock_step_16)) *
                    ONES_STEP_16 >>
                47;
            assert(where0 <= 16);
            const uint64_t first_bis = s[where0 + 2],
                           second_bis = s[where0 + 2 + 1];
            const int where1 =
                where0 * 8 +
                ((ULEQ_STEP_16(first_bis, rank_in_superblock_step_16) +
                  ULEQ_STEP_16(second_bis, rank_in_superblock_step_16)) *
                     ONES_STEP_16 >>
                 47);

            block_left += where1 * 4;
            count_left += where1;
            rank_in_block = rank - this->counts[count_left];
            assert(rank_in_block < 512);

        } else if (span < 256) {
            return ((uint16_t *)s)[rank % ones_per_inventory] + inventory_left;
        } else if (span < 512) {
            return ((uint32_t *)s)[rank % ones_per_inventory] + inventory_left;
        } else {
            return s[rank % ones_per_inventory];
        }

        const uint64_t rank_in_block_step_9 = rank_in_block * ONES_STEP_9;
        const uint64_t subcounts = this->counts[count_left + 1];
        const uint64_t offset_in_block =
            (ULEQ_STEP_9(subcounts, rank_in_block_step_9) * ONES_STEP_9 >> 54 &
             0x7);

        const uint64_t word = block_left + offset_in_block;
        const uint64_t rank_in_word =
            rank_in_block -
            (subcounts >> ((offset_in_block - 1) & 7) * 9 & 0x1FF);
        assert(offset_in_block <= 7);
        assert(rank_in_word < 64);
        return word * UINT64_C(64) +
               select64(this->m_bitset.data()[word], rank_in_word) + 1;
    }
};
}  // namespace Sealib

#endif  // SEALIB_DICTIONARY_RANK9SELECT_HPP_
