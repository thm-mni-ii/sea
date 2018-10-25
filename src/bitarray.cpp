#include "sealib/bitarray.h"
#include <cmath>
#include <cstdio>
#include <stdexcept>

using Sealib::BitArray;

void BitArray::insert(uint i, uint v) {
  uint gi = i / valuesPerGroup;
  uint vi = i % valuesPerGroup;
  uint s = (valuesPerGroup - vi - 1) * valueWidth;
  uint a = data[gi];
  a &= ~(valueMask << s);
  uint b = v << s;
  uint c = a | b;
  data[gi] = c;
}

static constexpr uint safeDiv(uint p1, uint p2) {
  return p2 == 0 ? 0 : p1 / p2;
}

BitArray::BitArray(uint size, uint values)
    : valueWidth(static_cast<uint>(ceil(log2(values)))),
      valuesPerGroup(safeDiv(8 * sizeof(uint), valueWidth)),
      valueMask((1 << valueWidth) - 1),
      data(new uint[safeDiv(size, valuesPerGroup) + 1]) {
  if (valueWidth >= sizeof(uint) * 8) {
    throw std::domain_error("v is too big (max v = bitsize(uint))");
  }
}

BitArray::~BitArray() { delete[] data; }
