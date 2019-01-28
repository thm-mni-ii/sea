#include <gtest/gtest.h>
#include <sealib/collection/compactarray.h>
#include <stdexcept>

using Sealib::CompactArray;

TEST(CompactArrayTest, insertAndGet) {
    CompactArray a(1500, 6);
    for (uint b = 0; b < 1500; b++) a.insert(b, 0);
    for (uint32_t b = 0; b < 50; b++) a.insert(b, 5);
    for (uint32_t b = 0; b < 50; b++) EXPECT_EQ(a.get(b), 5);
    for (uint32_t b = 50; b < 1500; b++) EXPECT_EQ(a.get(b), 0);
    a.insert(60, 1);
    EXPECT_EQ(a.get(60), 1);
    a.insert(1499, 1);
    EXPECT_EQ(a.get(1499), 1);
}

TEST(CompactArrayTest, alternate) {
    for (uint b = 2; b < static_cast<uint>(1 << (sizeof(uint) * 8 - 1));
         b *= 2) {
        uint n = 2500;
        CompactArray a(n, b);
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
