#ifndef SEALIB_COLLECTION_VARIANTBITSET_H_
#define SEALIB_COLLECTION_VARIANTBITSET_H_
#include <vector>
#include "sealib/_types.h"

namespace Sealib {
/**
 * Experimental bitset with block access.
 * It should be safe to access `bit` like a normal bit vector.
 * However, the other members support only a few safe operations:
 * - You can use the [] operator on any member.
 * - You can access the underlying array with data().
 * - You can use std::swap to swap contents of a member and an outside vector.
 *
 *  Example:
 *      VariantBitset b(20);
 *      b.bit[5]=1;
 *      b.byte[1]=20*b.bit[5];
 *      ...
 *
 * @author Simon Heuser
 */
union VariantBitset {
    /**
     * Member that allows reading and writing specific bits.
     */
    std::vector<bool> bit;
    /**
     * Member that allows reading and writing bytes.
     */
    std::vector<uint8_t> byte;
    /**
     * Member that allows reading and writing 64-bit words.
     */
    std::vector<uint64_t> word;

    /**
     * Creates a bitset of the given size.
     * @param size Number of bits in the bitset
     */
    VariantBitset(uint64_t size) : bit(size) {}

    /**
     * Creates a bitset from the given bit vector.
     * @param v bit vector to use
     */
    VariantBitset(std::vector<bool> &&v) : bit(std::move(v)) {}

    std::vector<bool>::reference operator[](uint64_t i) { return bit[i]; }
    bool operator[](uint64_t i) const { return bit[i]; }

    size_t size() const { return bit.size(); }

    uint8_t getBlock(uint64_t i) const { return byte[i]; }
    uint8_t getShiftedBlock(uint64_t i) const;

    uint64_t byteSize() const { return bit.capacity() / 8; }

    VariantBitset(VariantBitset const &);
    VariantBitset(VariantBitset &&);
    VariantBitset &operator=(VariantBitset const &);
    VariantBitset &operator=(VariantBitset &&);
    ~VariantBitset();
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_VARIANTBITSET_H_
