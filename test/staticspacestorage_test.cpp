#include "sealib/collection/staticspacestorage.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <random>
#include "../src/collection/simplesequence.h"

using Sealib::SimpleSequence;
using Sealib::StaticSpaceStorage;

TEST(StaticSpaceStorageTest, boundary) {
    std::vector<uint64_t> b = {59, 22, 55, 23, 3};
    std::vector<uint64_t> values = {576646035, 3360686, 193508993, 3833501, 3};
    StaticSpaceStorage s(StaticSpaceStorage::makeBitVector(b));

    for (uint64_t a = 0; a < values.size(); a++) {
        s.insert(a, values[a]);
    }
    for (uint64_t a = 0; a < values.size(); a++) {
        EXPECT_EQ(s.get(a), values[a]);
    }
}

TEST(StaticSpaceStorageTest, twoWords) {
    std::vector<bool> b;
    b.push_back(1);
    b.push_back(0);
    b.push_back(1);
    for (uint64_t a = 0; a < sizeof(uint64_t) * 8; a++) {
        b.push_back(0);
    }
    b.push_back(1);
    for (uint64_t a = 0; a < sizeof(uint64_t) * 8 - 3; a++) {
        b.push_back(0);
    }
    b.push_back(1);
    b.push_back(0);
    b.push_back(0);

    StaticSpaceStorage s(std::move(b));
    s.insert(1, 1UL << (sizeof(uint64_t) * 8 - 1));
    EXPECT_EQ(s.get(0), 0);
    EXPECT_EQ(s.get(1), 1UL << (sizeof(uint64_t) * 8 - 1));
    EXPECT_EQ(s.get(2), 0);
    s.insert(3, 3);
    EXPECT_EQ(s.get(3), 3);
}

TEST(StaticSpaceStorageTest, referenceTest) {
    uint64_t n = 100, d = sizeof(uint64_t) * 8;
    std::random_device rnd;
    std::uniform_int_distribution<uint64_t> dist(0, d - 1), dist2(0, n - 1);
    std::vector<uint64_t> sizes;
    for (uint64_t a = 0; a < n; a++) {
        sizes.emplace_back(dist(rnd));
    }
    StaticSpaceStorage s(StaticSpaceStorage::makeBitVector(sizes));
    SimpleSequence<uint64_t> c(n);
    // insert random values < size[a]
    for (uint64_t a = 0; a < n; a++) {
        if (sizes[a] > 1) {
            std::uniform_int_distribution<uint64_t> valR(1, sizes[a] - 1);
            uint64_t b = valR(rnd);
            s.insert(a, b);
            c.insert(a, b);
        }
    }
    // compare sequential
    for (uint64_t a = 0; a < n; a++) {
        ASSERT_EQ(s.get(a), c.get(a)) << "failed at index " << a;
    }
    // compare random indices (5000x)
    for (uint64_t a = 0; a < 5000; a++) {
        uint64_t b = dist2(rnd);
        EXPECT_EQ(s.get(b), c.get(b));
    }
}
