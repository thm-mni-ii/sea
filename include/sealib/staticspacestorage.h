#ifndef SEALIB_STATICSPACESTORAGE_H_
#define SEALIB_STATICSPACESTORAGE_H_
#include <cstdint>
#include <vector>
#include "sealib/_types.h"
#include "sealib/bitset.h"
#include "sealib/rankselect.h"

namespace Sealib {
/**
 * Data storage created by static space allocation. A bit pattern of n+N bits
 * shows the bit size of each entry.
 * EFFICIENCY: O(n+N) bits
 */
class StaticSpaceStorage {
 public:
    /**
     * @param i index of the storage array
     * @return value stored in element i
     */
    uint get(uint i) const;

    /**
     * @param i index of the storage array
     * @param v value to insert
     */
    void insert(uint i, uint v);

    /**
     * Create a new storage from a bit vector.
     * @param bits bit pattern that shows the position and size of entries
     *  Example:
     *      10001001100
     *      => 4 data packs: size 3, 2, 0 and 2 bits
     */
    explicit StaticSpaceStorage(const std::vector<bool> &bits);

    /**
     * Convenience method to create a bit pattern from a vector of sizes
     * @param sizes a vector that holds sizes of each entry (e.g. {2,3,6,2})
     * @return bit pattern corresponding to the input vector (e.g.
     * 10010001000000100)
     */
    static std::vector<bool> makeBitVector(std::vector<uint> *sizes);

 private:
    const uint n;
    Bitset<uint_fast8_t> pattern;
    const RankSelect rankSelect;
    Bitset<uint_fast8_t> storage;

    constexpr unsigned long getEnd(uint k) const {
        return (k < n) ? rankSelect.select(k + 1) : (n + storage.size() + 1);
    }
};
}  // namespace Sealib
#endif  // SEALIB_STATICSPACESTORAGE_H_
