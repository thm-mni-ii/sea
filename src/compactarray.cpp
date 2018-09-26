#include "sealib/compactarray.h"
#include <iostream>

using Sealib::CompactArray;

void CompactArray::insert(uint i, unsigned int p) {
  // values per group: vpg, value width=ceil(ld(v)) bits, group width=vpg*vw
  if (ceil(log2(p)) > valueWidth) {
    std::cout << "failing because " << log2(p) << " bits is wider than "
              << valueWidth << " bits\n";
    throw TOOLARGE;
  }
  unsigned groupOffset =
      static_cast<unsigned>(floor(i / static_cast<double>(valuesPerGroup)));
  if (groupOffset >= groupCount) throw OUTOFBOUNDS;
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  Group a = *(group[groupOffset]);
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  Group b(groupWidth);
  b.setBlock(0, maxValue << gap);
  Group c = a & ~b;
  Group d(groupWidth);
  d.setBlock(0, p << gap);
  Group r = c | d;
  delete group[groupOffset];
  group[groupOffset] = new Group(r);
}

unsigned int CompactArray::get(uint i) {
  unsigned groupOffset = static_cast<unsigned>(floor(i / valuesPerGroup));
  if (groupOffset >= groupCount) throw OUTOFBOUNDS;
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  unsigned a = static_cast<unsigned>(group[groupOffset]->getBlock(0));
  unsigned b = maxValue << gap;
  unsigned c = a & b;
  unsigned d = c >> gap;
  return d;
}

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
  groupCount =
      static_cast<unsigned>(ceil(count / static_cast<double>(valuesPerGroup)));
  group = new Group *[groupCount];
  for (unsigned a = 0; a < groupCount; a++) group[a] = new Group(groupWidth);
}

CompactArray::~CompactArray() { delete[] group; }
