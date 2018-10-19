#include "sealib/compactarray.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

using Sealib::CompactArray;

static void E_outofbounds(uint i) {
  std::stringstream e;
  e << "compactarray: index out of bounds\n"
    << "tried to access index " << i << "\n";
  throw std::out_of_range(e.str());
}
static void E_toolarge(uint i, unsigned int p) {
  std::stringstream e;
  e << "compactarray: inserted value is too large\n"
    << "tried to insert " << p << " at index " << i << " (" << ceil(log2(p))
    << "bits wide)\n";
  throw std::invalid_argument(e.str());
}

void CompactArray::insert(uint i, unsigned int p) {
  // values per group: vpg, value width=ceil(ld(v)) bits, group width=vpg*vw
  if (ceil(log2(p)) > valueWidth) {
    E_toolarge(i, p);
  }
  unsigned groupOffset =
      static_cast<unsigned>(floor(i / static_cast<double>(valuesPerGroup)));
  if (groupOffset >= groupCount) E_outofbounds(i);
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  Group a = group[groupOffset];
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  Group b(groupWidth);
  b.setBlock(0, maxValue << gap);
  Group c = a & ~b;
  Group d(groupWidth);
  d.setBlock(0, p << gap);
  group[groupOffset] = c | d;
}

unsigned int CompactArray::get(uint i) {
  unsigned groupOffset = static_cast<unsigned>(floor(i / valuesPerGroup));
  if (groupOffset >= groupCount) E_outofbounds(i);
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  unsigned a = static_cast<unsigned>(group[groupOffset].getBlock(0));
  unsigned b = maxValue << gap;
  unsigned c = a & b;
  unsigned d = c >> gap;
  return d;
}

unsigned int CompactArray::size() { return valueCount; }

CompactArray::CompactArray(unsigned int count, unsigned int v) {
  /**
   * value width: no. bits a value occupies (e.g. ceil(ld(3)) for 3 possible
   * states)
   * group width: no. bits a group occupies (e.g. ceil(valueWidth*3/e) for 3
   * possible states)
   * values per group: vw*vpg = bitsize(unsigned int) <=> vpg=bitsize(unsigned
   * int)/vw
   */

  // the following is valid if the inserted values can have v states:
  valueWidth = static_cast<unsigned>(ceil(log2(v)));
  valuesPerGroup =
      static_cast<unsigned>((sizeof(unsigned int) * 8) / valueWidth);
  groupWidth = valuesPerGroup *
               valueWidth;  // bits for a group of vpg (e.g. 2) consec. colors
  maxValue = static_cast<unsigned>(pow(2, valueWidth) - 1);
  valueCount = count;
  groupCount =
      static_cast<unsigned>(ceil(count / static_cast<double>(valuesPerGroup)));
  group = std::vector<Group>(groupCount, Group(groupWidth));
}

CompactArray::~CompactArray() {}
