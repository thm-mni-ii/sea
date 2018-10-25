#include "sealib/compactarray.h"
#include <math.h>
#include <stdexcept>

using Sealib::CompactArray;

static const std::out_of_range OUTOFBOUNDS =
    std::out_of_range("compactarray: index out of bounds");
static const std::invalid_argument TOOLARGE =
    std::invalid_argument("compactarray: inserted value is too large");
void CompactArray::insert(uint i, uint32_t p) {
  // values per group: vpg, value width=ceil(ld(v)) bits, group width=vpg*vw
  if (ceil(log2(p)) > valueWidth) {
    throw TOOLARGE;
  }
  uint32_t groupOffset =
      static_cast<unsigned>(floor(i / static_cast<double>(valuesPerGroup)));
  if (groupOffset >= groupCount) throw OUTOFBOUNDS;
  uint32_t valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  Group a = *(group[groupOffset]);
  uint32_t gap =
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

uint32_t CompactArray::get(uint i) {
  uint32_t groupOffset = static_cast<unsigned>(floor(i / valuesPerGroup));
  if (groupOffset >= groupCount) throw OUTOFBOUNDS;
  uint32_t valueOffset = static_cast<unsigned>(fmod(i, valuesPerGroup));
  uint32_t gap =
      static_cast<unsigned>((valuesPerGroup - valueOffset - 1) * valueWidth);
  uint32_t a = static_cast<unsigned>(group[groupOffset]->getBlock(0));
  uint32_t b = maxValue << gap;
  uint32_t c = a & b;
  uint32_t d = c >> gap;
  return d;
}

CompactArray::CompactArray(uint32_t count, uint32_t v) {
  /**
   * value width: no. bits a value occupies (e.g. ceil(ld(3)) for 3 possible
   * states)
   * group width: no. bits a group occupies (e.g. ceil(valueWidth*3/e) for 3
   * possible states)
   * values per group: vw*vpg = bitsize(uint32_t ) <=> vpg=bitsize(unsigned
   * int)/vw
   */

  // the following is valid if the inserted values can have v states:
  valueWidth = static_cast<unsigned>(ceil(log2(v)));
  valuesPerGroup = static_cast<unsigned>((sizeof(uint32_t) * 8) / valueWidth);
  groupWidth = valuesPerGroup *
               valueWidth;  // bits for a group of vpg (e.g. 2) consec. colors
  maxValue = static_cast<unsigned>(pow(2, valueWidth) - 1);
  groupCount =
      static_cast<unsigned>(ceil(count / static_cast<double>(valuesPerGroup)));
  group = new Group *[groupCount];
  for (uint32_t a = 0; a < groupCount; a++) group[a] = new Group(groupWidth);
}

CompactArray::~CompactArray() { delete[] group; }
