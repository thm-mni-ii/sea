#include "sealib/compactarray.h"
#include <stdio.h>
Group **group;
double e;
/**
 * value width: no. bits a value occupies (e.g. ceil(ld(3)) for 3 possible
 * states)
 * group width: no. bits a group occupies (e.g. ceil(valueWidth*3/e) for 3
 * possible states)
 */
unsigned valueWidth, maxValue, groupWidth;
double valuesPerGroup;
unsigned groupCount;

void CompactArray::insert(unsigned int i, unsigned int p) {
  // values per group: 3/e, value width=ceil(log3) bits, group width
  unsigned groupOffset = static_cast<unsigned>(floor(i / valuesPerGroup));
  if (groupOffset >= groupCount) throw COMPACTARRAY_FAULT;
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  Group a = *(group[groupOffset]);
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  Group b = Group(groupWidth, maxValue << gap);
  Group c = a & ~b;
  Group d = Group(groupWidth, p << gap);
  Group r = c | d;
  delete group[groupOffset];
  group[groupOffset] = new Group(r);
}

unsigned int CompactArray::get(unsigned int i) {
  unsigned groupOffset = static_cast<unsigned>(floor(i / valuesPerGroup));
  if (groupOffset >= groupCount) throw COMPACTARRAY_FAULT;
  unsigned valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  unsigned gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  unsigned a = static_cast<unsigned>(group[groupOffset]->to_ulong());
  unsigned b = maxValue << gap;
  unsigned c = a & b;
  unsigned d = c >> gap;
  return d;
}

CompactArray::CompactArray(unsigned int count, double epsilon) {
  e = epsilon;
  // the following is valid if the inserted values can have 3 states:
  valueWidth = static_cast<unsigned>(ceil(log(3) / log(2)));
  valuesPerGroup = ceil(3 / e);
  groupWidth = static_cast<unsigned>(
      valuesPerGroup *
      valueWidth);  // bits for a group of 3/e (e.g. 2) consec. colors
  maxValue = static_cast<unsigned>(pow(2, valueWidth) - 1);
  groupCount = static_cast<unsigned>(ceil(count / (groupWidth / valueWidth)));
  printf("e=%3.2f, vw=%u, vpg=%.0f, maxv=0x%x\n", e, valueWidth, valuesPerGroup,
         maxValue);
  group = new Group *[groupCount];
  for (unsigned a = 0; a < groupCount; a++) group[a] = new Group(groupWidth, 0);
}

CompactArray::~CompactArray() { delete[] group; }
