#include "sealib/bitarray.h"
#include <gtest/gtest.h>

using Sealib::BitArray;

TEST(BitArray, alternate) {
  for (uint b = 2; b < static_cast<uint>(1 << (sizeof(uint) * 8 - 1)); b *= 2) {
    uint n = 2500;
    BitArray a(n, b);
    for (uint c = 0; c < n; c++) {
      if (c % 2 == 0)
        a.insert(c, 1);
      else
        a.insert(c, b - 1);
    }
    for (uint c = 0; c < n; c++) {
      if (c % 2 == 0)
        EXPECT_EQ(a.get(c), 1);
      else
        EXPECT_EQ(a.get(c), b - 1);
    }
  }
}
