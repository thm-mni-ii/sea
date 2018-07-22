#ifndef SEALIB_COMPACTARRAY_H_
#define SEALIB_COMPACTARRAY_H_

#include <math.h>
#include <third-party/boost/include/boost/dynamic_bitset.hpp>

typedef boost::dynamic_bitset<> Group;

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
 public:
  /**
   * Create a new compact array.
   * @param count number of values this compact array can hold
   * @param vpg values per group: the larger vpg is, the more values are put
   * into one group. It does not affect the total space needed.
   * The space efficiency is optimal if vpg divides n.
   */
  CompactArray(unsigned int count, unsigned int vpg);

  ~CompactArray();

  /**
   * Insert a value to the given index.
   * @param i the destination index
   * @param p the value to insert (in [0,3])
   */
  void insert(unsigned int i, unsigned int p);

  /**
   * Get a value from the compact array.
   * @param i index to get the value from
   * @return the found value
   */
  unsigned int get(unsigned int i);
};
}  // namespace Sealib
#endif  // SEALIB_COMPACTARRAY_H_
