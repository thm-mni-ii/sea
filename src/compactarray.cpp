#include "sealib/compactarray.h"

using Sealib::CompactArray;

void CompactArray::insert(uint i, unsigned int p) {
  // values per group: 3/e, value width=ceil(log3) bits, group width
  unsigned groupOffset =
      static_cast<unsigned>(floor(i / static_cast<double>(valuesPerGroup)));
  if (groupOffset >= groupCount) throw OUTOFBOUNDS;
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  Group a = *(group[groupOffset]);
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  Group b = Group(groupWidth);
  b.setBlock(0,maxValue<<gap);
  Group c = a & ~b;
  Group d = Group(groupWidth);
  d.setBlock(0,p<<gap);
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

CompactArray::CompactArray(unsigned int count, unsigned int vpg) {
  /**
   * value width: no. bits a value occupies (e.g. ceil(ld(3)) for 3 possible
   * states)
   * group width: no. bits a group occupies (e.g. ceil(valueWidth*3/e) for 3
   * possible states)
   */

  // the following is valid if the inserted values can have 3 states:
  valueWidth = static_cast<unsigned>(ceil(log(3) / log(2)));
  valuesPerGroup = vpg;
  groupWidth = valuesPerGroup *
               valueWidth;  // bits for a group of 3/e (e.g. 2) consec. colors
  maxValue = static_cast<unsigned>(pow(2, valueWidth) - 1);
  groupCount =
      static_cast<unsigned>(ceil(count / static_cast<double>(valuesPerGroup)));
  group = new Group *[groupCount];
  for (unsigned a = 0; a < groupCount; a++) group[a] = new Group(groupWidth);
}

CompactArray::~CompactArray() { delete[] group; }
