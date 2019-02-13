#ifndef SEALIB_COLLECTION_COMPACTARRAY_H_
#define SEALIB_COLLECTION_COMPACTARRAY_H_

#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/bitset.h"
#include "sealib/collection/sequence.h"

using Sealib::Bitset;

namespace Sealib {
/**
 * Groups of bits, packed into an array of uints.
 * To get a bitset, create with v=2. To get a color vector, create with v=3 or
 * v=4.
 * For optimal space usage, see constructor comments.
 * @author Simon Heuser
 */
class CompactArray : public Sequence<uint64_t> {
 public:
    /**
     * Create a new compact array.
     * @param size number of values the compact array should hold
     * @param v number of states for one value (e.g. v=2 for a binary array, v=4
     * for states 0,1,2,3). Recommended: ld(v) should divide bitsize(uint64_t) (e.g.
     * groups of 4 bits fit into a 32 bit integer perfectly)
     * @throws std::domain_error when ld(v) exceeds bitsize(uint64_t)
     */
    explicit CompactArray(uint64_t count, uint64_t v = 3);

    /**
     * Insert a value to the given index.
     * @param i the destination index
     * @param p the value to insert (in [0,v])
     */
    void insert(uint64_t i, uint64_t p) override;

    /**
     * Get a value from the compact array.
     * @param i index to get the value from
     * @return the found value
     */
    uint64_t get(uint64_t i) const override;

 private:
    uint64_t valueWidth, singleMask;
    std::vector<uint64_t> data;
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_COMPACTARRAY_H_
