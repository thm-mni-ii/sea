#include "sealib/bitarray.h"
#include <cmath>
#include <cstdio>
#include <stdexcept>

using Sealib::BitArray;

void BitArray::insert(uint i, uint v) {
  uint gi = i / valuesPerGroup;
  uint vi = i & (valuesPerGroup - 1);
  uint s = (valuesPerGroup - vi - 1) * valueWidth;
  uint a = data[gi];
  a &= ~(valueMask << s);
  uint b = v << s;
  uint c = a | b;
  data[gi] = c;
}

uint BitArray::get(uint i) {
  uint gi = i / valuesPerGroup;
  uint vi = i & (valuesPerGroup - 1);
  uint a = data[gi];
  uint b = a >> ((valuesPerGroup - vi - 1) * valueWidth);
  uint c = b & valueMask;
  return c;
}

static constexpr uint safeDiv(uint p1, uint p2) {
  return p2 == 0 ? 0 : p1 / p2;
}

BitArray::BitArray(uint size, uint values)
    : valueWidth(static_cast<uint>(ceil(log2(values)))),
      valuesPerGroup(safeDiv(8 * sizeof(uint), valueWidth)),
      valueMask((1 << valueWidth) - 1),
      data(safeDiv(size, valuesPerGroup)) {
  if (valueWidth + 1 >= sizeof(uint) * 8) {
    throw std::domain_error("v is too big (max v = bitsize(uint))");
  }
}
