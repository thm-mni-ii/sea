#include "sealib/bitarray.h"
#include <cmath>
#include <cstdio>
#include <stdexcept>

using Sealib::BitArray;

void BitArray::insert(uint i, uint8_t v) {
  uint gi = i / valuesPerGroup;
  uint8_t vi = i & (valuesPerGroup - 1);
  uint8_t s = (valuesPerGroup - vi - 1) * valueWidth;
  uint8_t a = data[gi];
  a &= ~(valueMask << s);
  uint8_t b = v << s;
  uint8_t c = a | b;
  data[gi] = c;
}

uint8_t BitArray::get(uint i) {
  uint gi = i / valuesPerGroup;
  uint8_t vi = i & (valuesPerGroup - 1);
  uint8_t a = data[gi];
  uint8_t b = a >> ((valuesPerGroup - vi - 1) * valueWidth);
  uint8_t c = b & valueMask;
  return c;
}

BitArray::BitArray(uint size, uint8_t values)
    : data(size),
      maxValue(values - 1),
      valueWidth(static_cast<uint8_t>(ceil(log2(values)))),
      valueMask((1 << valueWidth) - 1) {
  if (values < 2) {
    throw std::domain_error("v < 2, maybe an overflow happened? (max v: 255)");
  } else {
    valuesPerGroup = 8 * sizeof(uint8_t) / valueWidth;
  }
}
