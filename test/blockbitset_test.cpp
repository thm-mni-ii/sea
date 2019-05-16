#include "sealib/collection/blockbitset.h"
#include <gtest/gtest.h>

using namespace Sealib;  // NOLINT

TEST(BlockBitsetTest, getAndSet) {
    BlockBitset b(100);
    EXPECT_EQ(b.bit.size(), 100);
    EXPECT_EQ(b.byte.data(), (uint8_t*)b.word.data());
    std::vector<uint64_t> v = {12, 24};
    std::swap(v, b.word);
    EXPECT_EQ(b.word[0], 12);
    EXPECT_EQ(b.word[1], 24);
    std::swap(v, b.word);

    b.bit[3] = 1;
    EXPECT_EQ(b.bit[3], 1);
    EXPECT_EQ(b.byte[0], 0x8);
    EXPECT_EQ(b.word[0], 0x8);

    b.byte[1] = 0x2;
    EXPECT_EQ(b.bit[9], 1);
    EXPECT_EQ(b.word[0], 0x208);

    b.word[1] = 0x1;
    EXPECT_EQ(b.bit[64], 1);
    EXPECT_EQ(b.byte[8], 0x1);
}
