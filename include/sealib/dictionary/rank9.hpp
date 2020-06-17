#ifndef SEALIB_DICTIONARY_RANK9_HPP_
#define SEALIB_DICTIONARY_RANK9_HPP_

#include <sealib/collection/bitset.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace Sealib {

/** A class implementing Rank9, a ranking structure using
 * 25% additional space and providing very fast ranking.
 *
 * Reference implementation by Sebastiano Vigna in the sux library:
 * https://github.com/vigna/sux
 *
 * Related Paper:
 * Sebastiano Vigna. 2008. Broadword implementation of rank/select queries. In
 * Proceedings of the 7th international conference on Experimental algorithms
 * (WEA’08). Springer-Verlag, Berlin, Heidelberg, 154–168.
 */
class Rank9 {
 private:
    void init() {
        const uint64_t num_words = (m_bitset.size() + 63) / 64;
        const uint64_t num_counts =
            ((m_bitset.size() + 64 * 8 - 1) / (64 * 8)) * 2;

        // Init rank structure
        counts.resize(num_counts + 2);

        num_ones = 0;
        uint64_t pos = 0;
        auto bits = m_bitset.data();
        for (uint64_t i = 0; i < num_words; i += 8, pos += 2) {
            counts[pos] = num_ones;
            num_ones += __builtin_popcountll(bits[i]);
            for (int j = 1; j < 8; j++) {
                counts[pos + 1] |= (num_ones - counts[pos]) << 9 * (j - 1);
                if (i + j < num_words)
                    num_ones += __builtin_popcountll(bits[i + j]);
            }
        }

        counts[num_counts] = num_ones;

        _max_rank = rank(m_bitset.size());
        assert(num_ones <= m_bitset.size());
    }

 protected:
    size_t num_ones;
    uint64_t _max_rank;
    Sealib::Bitset<uint64_t> m_bitset;
    std::vector<uint64_t> counts;

 public:
    /** Creates a new instance using a given bit vector.
     *
     * **Warning**: if the bitset has bits set beyond it's own size()
     * the behaviour is undefined (aka everything explodes)
     *
     * @param bitset a Sealib::Bitset of 64-bit words.
     */
    explicit Rank9(Sealib::Bitset<uint64_t>  bitset)
        : num_ones(0), _max_rank(0), m_bitset(std::move(bitset)), counts() {
        init();
    }
    explicit Rank9(Sealib::Bitset<uint64_t>&& bitset)
        : num_ones(0), _max_rank(0), m_bitset(bitset), counts() {
        init();
    }

    /** Returns the number of ones before the given posistion.
     *
     * @param pos A position from 0 to size() (included).
     * @return the number of ones from the start of the bit vector up to the
     * given position (excluded).
     */
    uint64_t rank(const size_t k) {
        assert(k <= m_bitset.size());
        const auto bits = m_bitset.data();
        const uint64_t word = k / 64;
        const uint64_t block = word / 4 & ~1;
        const int offset = word % 8 - 1;
        return counts[block] +
               (counts[block + 1] >>
                    (offset + (offset >> (sizeof offset * 8 - 4) & 0x8)) * 9 &
                0x1FF) +
               __builtin_popcountll(bits[word] & ((1ULL << k % 64) - 1));
    }

    /** @return the size in bits of the underlying bit vector. */
    size_t size() const { return m_bitset.size(); }

    /** @return the number of bits set to 1 in the bitset, aka max rank value.
     */
    size_t max_rank() const { return _max_rank; }
};
}  // namespace Sealib

#endif  // SEALIB_DICTIONARY_RANK9_HPP_
