#ifndef SEALIB_COMPACTARRAY_H_
#define SEALIB_COMPACTARRAY_H_

#include <math.h>
#include <third-party/boost/include/boost/dynamic_bitset.hpp>

typedef boost::dynamic_bitset<> Group;

/**
 * This compact array stores a given number of values in as low space as
 * possible.
 * Therefore an "epsilon" parameter is used: the smaller epsilon is, the larger
 * the storage groups will be.
 * @author Simon Heuser
 */
namespace Sealib {
class CompactArray {
 private:
  std::out_of_range OUTOFBOUNDS = std::out_of_range("index out of bounds");

 public:
  /**
   * Create a new compact array.
   * @param count number of values this compact array can hold
   * @param epsilon width parameter: 3/e should divide n
   * If you don't know what to use for epsilon: 1.5 if count is even, 3.0 if
   * count is odd.
   */
  CompactArray(unsigned int count, double epsilon);

  ~CompactArray();

  /**
   * Insert a value to the given index.
   * @param i the destination index
   * @param p the value to insert
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
