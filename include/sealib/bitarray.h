#ifndef SEALIB_BITARRAY_H_
#define SEALIB_BITARRAY_H_
#include <vector>
#include "sealib/_types.h"

namespace Sealib {
class BitArray {
 public:
  /**
   * Insert a value into the bit array.
   * @param i index to insert into
   * @param v value to insert
   */
  void insert(uint i, uint8_t v);

  /**
   * Get a value from the bit array.
   * @param i index to get the value from
   * @return value stored at the given index
   */
  uint8_t get(uint i);

  /**
   * Construct a bit array. Recommended: v is divisible by bitsize(uint8_t)=8
   * @param size number of values the bit array should hold
   * @param v number of states for one value (e.g. v=2 for a binary array, v=4
   * for states 0,1,2,3)
   */
  BitArray(uint size, uint8_t v);

 private:
  std::vector<uint8_t> data;
  uint8_t maxValue, valueWidth, valuesPerGroup, valueMask;
};
}  // namespace Sealib
#endif  // SEALIB_BITARRAY_H_
