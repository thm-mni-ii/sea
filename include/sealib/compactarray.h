#ifndef SEALIB_COMPACTARRAY_H_
#define SEALIB_COMPACTARRAY_H_

#include <memory>
#include "sealib/_types.h"
#include "sealib/bitset.h"

using Sealib::Bitset;

namespace Sealib {
/**
 * Groups of bits, packed into an array of uints.
 * To get a bitset, create with v=2. To get a color vector, create with v=3 or
 * v=4.
 * For optimal space usage, see constructor comments.
 * @author Simon Heuser
 */
class CompactArray {
 public:
  /**
   * Create a new compact array.
   * @param size number of values the compact array should hold
   * @param v number of states for one value (e.g. v=2 for a binary array, v=4
   * for states 0,1,2,3). Recommended: ld(v) should divide bitsize(uint) (e.g.
   * groups of 4 bits fit into a 32 bit integer perfectly)
   * @throws std::domain_error when ld(v) exceeds bitsize(uint)
   */
  explicit CompactArray(uint count, uint v = 3);

  /**
   * Insert a value to the given index.
   * @param i the destination index
   * @param p the value to insert (in [0,v])
   */
  void insert(uint i, uint p);

/**
 * Get a value from the compact array.
 * @param i index to get the value from
 * @return the found value
 */
#ifdef __clang__
  constexpr uint get(uint i) const {
    return (data[i / valuesPerGroup] >>
            ((valuesPerGroup - (i % valuesPerGroup) - 1) * valueWidth)) &
           valueMask;
  }
#else
  uint get(uint i) const;
#endif

  unsigned int size();

 private:
  const uint valueWidth, valuesPerGroup, valueMask;
  std::unique_ptr<uint[]> data;
};
}  // namespace Sealib
#endif  // SEALIB_COMPACTARRAY_H_
