#include "sealib/bitarray.h"
#include <gtest/gtest.h>

using Sealib::BitArray;

TEST(BitArray, byteSized) {
  for (uint8_t b = 2; b < 255; b++) {
    BitArray a(2500, b);
    a.insert(50, b - 1);
    EXPECT_EQ(a.get(50), b - 1);
    a.insert(49, 1);
    EXPECT_EQ(a.get(49), 1);
    EXPECT_EQ(a.get(50), b - 1);
  }
  EXPECT_ANY_THROW(BitArray a(2500, static_cast<uint8_t>(256)));
}
