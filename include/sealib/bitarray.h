#ifndef SEALIB_BITARRAY_H_
#define SEALIB_BITARRAY_H_
#include <vector>
#include <memory>
#include "sealib/_types.h"

namespace Sealib {
/**
 * A sequence of integers which hold packed bit groups.
 * To get a bitset, create with v=2. To get a color vector, create with v=3 or
 * v=4.
 * For optimal space usage, see constructor comments.
 */
class BitArray {
 public:
  /**
   * Insert a value into the bit array.
   * @param i index to insert into
   * @param v value to insert
   * @throws nothing, but might die when going out of bounds
   */
  void insert(uint i, uint v);

  /**
   * Get a value from the bit array.
   * @param i index to get the value from
   * @return value stored at the given index
   * @throws nothing, but might die when going out of bounds
   */
  constexpr uint get(uint i) const {
    return (data[i / valuesPerGroup] >>
            ((valuesPerGroup - (i % valuesPerGroup) - 1) * valueWidth)) &
           valueMask;
  }

  /**
   * Construct a bit array with the given size and value states.
   * @param size number of values the bit array should hold
   * @param v number of states for one value (e.g. v=2 for a binary array, v=4
   * for states 0,1,2,3). Recommended: ld(v) should divide bitsize(uint) (e.g.
   * groups of 4 bits
   * fit into a 32 bit integer perfectly)
   * @throws std::domain_error when ld(v) exceeds bitsize(uint)
   */
  BitArray(uint size, uint v);

 private:
  const uint valueWidth, valuesPerGroup, valueMask;
  std::unique_ptr<uint[]> data;
};
}  // namespace Sealib
#endif  // SEALIB_BITARRAY_H_
