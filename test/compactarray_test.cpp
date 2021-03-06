#include <gtest/gtest.h>
#include <sealib/collection/compactarray.h>
#include <stdexcept>

using Sealib::CompactArray;

// Do some inserts and verify that the values are present
// Also checks that the array is zero-initialized
TEST(CompactArrayTest, insertAndGet) {
    CompactArray a(1500, 6);
    for (uint32_t b = 0; b < 50; b++) a.insert(b, 5);
    for (uint32_t b = 0; b < 50; b++) EXPECT_EQ(a.get(b), 5);
    for (uint32_t b = 50; b < 1500; b++) EXPECT_EQ(a.get(b), 0);
    a.insert(60, 1);
    EXPECT_EQ(a.get(60), 1);
    a.insert(1499, 1);
    EXPECT_EQ(a.get(1499), 1);
}

// Insert 1 and MAX into the elements of an array in alternating order and
// verify that the values are present
// (where MAX is the maximum value possible for the current element)
// This test checks that no writes occur across the boundaries of an element
TEST(CompactArrayTest, alternate) {
    for (uint64_t b = 2;
         b < static_cast<uint64_t>(1UL << (sizeof(uint64_t) * 8 - 1)); b *= 2) {
        uint64_t n = 2500;
        CompactArray a(n, b);
        for (uint64_t c = 0; c < n; c++) {
            if (c % 2 == 0)
                a.insert(c, 1);
            else
                a.insert(c, b - 1);
        }
        for (uint64_t c = 0; c < n; c++) {
            if (c % 2 == 0)
                EXPECT_EQ(a.get(c), 1);
            else
                EXPECT_EQ(a.get(c), b - 1);
        }
    }
}
