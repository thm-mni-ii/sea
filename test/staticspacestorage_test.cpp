#include "sealib/collection/staticspacestorage.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <random>
#include "../src/collection/simplesequence.h"

using Sealib::StaticSpaceStorage;
using Sealib::SimpleSequence;

TEST(StaticSpaceStorageTest, boundary) {
    std::vector<uint> b = {59, 22, 55, 23, 3};
    std::vector<uint64_t> values = {576646035, 3360686, 193508993, 3833501, 3};
    StaticSpaceStorage s(StaticSpaceStorage::makeBitVector(&b));

    for (uint a = 0; a < values.size(); a++) {
        s.insert(a, values[a]);
    }
    for (uint a = 0; a < values.size(); a++) {
        EXPECT_EQ(s.get(a), values[a]);
    }
}

TEST(StaticSpaceStorageTest, referenceTest) {
    uint n = 100, d = sizeof(uint64_t) * 8;
    std::random_device rnd;
    std::uniform_int_distribution<uint> dist(0, d - 1), dist2(0, n - 1);
    std::vector<uint> sizes;
    for (uint a = 0; a < n; a++) {
        sizes.emplace_back(dist(rnd));
    }
    StaticSpaceStorage s(StaticSpaceStorage::makeBitVector(&sizes));
    SimpleSequence<uint64_t> c(n);
    // insert random values < size[a]
    for (uint a = 0; a < n; a++) {
        if (sizes[a] > 0) {
            uint64_t b =
                static_cast<uint64_t>(fmod(rnd(), pow(2, sizes[a]) - 1));
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
