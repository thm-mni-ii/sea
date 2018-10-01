#ifndef SEALIB_COMPACTARRAY_H_
#define SEALIB_COMPACTARRAY_H_

#include "sealib/_types.h"
#include "sealib/bitset.h"

using Sealib::Bitset;

namespace Sealib {
/**
 * This compact array stores a given number of values in as low space as
 * possible. Each inserted value must be in the range [0,3].
 * Therefore, the values are stored inside groups: with 3/ε values per group,
 * each group requires ld(3)*3/ε bits. (with ceiling where appropriate)
 * The total space requirement is n/(3/ε)*ld(3)*3/ε = ld(3)*n bits.
 * @author Simon Heuser
 */
class CompactArray {
  typedef Bitset<uint> Group;

 public:
  /**
   * Create a new compact array.
   * @param count number of values this compact array can hold
   * @param v the highest possible value (default: 3)
   */
  explicit CompactArray(unsigned int count, unsigned int v = 3);

  ~CompactArray();

  /**
   * Insert a value to the given index.
   * @param i the destination index
   * @param p the value to insert (in [0,v])
   */
  void insert(uint i, unsigned int p);

  /**
   * Get a value from the compact array.
   * @param i index to get the value from
   * @return the found value
   */
  unsigned int get(uint i);

  unsigned int size();

 private:
  unsigned valueWidth, maxValue, groupWidth, valuesPerGroup;
  unsigned valueCount, groupCount;

  Group **group;
};
}  // namespace Sealib
#endif  // SEALIB_COMPACTARRAY_H_
