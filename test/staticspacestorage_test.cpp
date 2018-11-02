#include "sealib/staticspacestorage.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <random>
#include "src/simplecontainer.h"

using Sealib::StaticSpaceStorage;
using Sealib::SimpleContainer;

TEST(StaticSpaceStorageTest, boundary) {
    std::vector<uint> b = {3, 1, 4, 60, 2};
    StaticSpaceStorage s(StaticSpaceStorage::makeBitVector(&b));
    s.insert(0, 5);
    EXPECT_EQ(s.get(0), 5);
    s.insert(1, 1);
    EXPECT_EQ(s.get(1), 1);
    s.insert(2, 15);
    EXPECT_EQ(s.get(2), 15);
    s.insert(4, 3);
    s.insert(3, static_cast<uint64_t>(2.5e9) + 1);
    EXPECT_EQ(s.get(3), static_cast<uint64_t>(2.5e9) + 1);
    EXPECT_EQ(s.get(4), 3);
    EXPECT_EQ(s.get(2), 15);
    EXPECT_EQ(s.get(1), 1);
    EXPECT_EQ(s.get(0), 5);
}

TEST(StaticSpaceStorageTest, referenceTest) {
    uint n = 1000, d = sizeof(uint64_t) * 8;
    std::random_device rnd;
    std::uniform_int_distribution<uint> dist(0, d - 1), dist2(0, n - 1);
    std::vector<uint> sizes;
    for (uint a = 0; a < n; a++) {
        sizes.emplace_back(dist(rnd));
    }
    StaticSpaceStorage s(StaticSpaceStorage::makeBitVector(&sizes));
    SimpleContainer<uint64_t> c(n);
    // insert random values < size[a]
    for (uint a = 0; a < n; a++) {
        if (sizes[a] > 0) {
            uint64_t b =
                static_cast<uint64_t>(fmod(rnd(), pow(2, sizes[a]) - 1));
            printf("sizes[%u]=%u, b=%u\n", a, sizes[a], b);
            s.insert(a, b);
            c.insert(a, b);
        }
    }
    // compare sequential
    for (uint a = 0; a < n; a++) {
        ASSERT_EQ(s.get(a), c.get(a)) << "failed at index " << a;
    }
    // compare random indices (5000x)
    for (uint a = 0; a < 5000; a++) {
        uint b = dist2(rnd);
        EXPECT_EQ(s.get(b), c.get(b));
    }
}
