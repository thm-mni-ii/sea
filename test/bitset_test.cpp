#include <gtest/gtest.h>
#include <sealib/collection/bitset.h>

using Sealib::Bitset;

TEST(BitsetTest, BitsetFunctionality_UnsignedLong) {
    typedef uint64_t block;

    Bitset<> bs;
    ASSERT_EQ(bs.size(), 0);
    // always one buffer block
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(1);
    ASSERT_EQ(bs.size(), 1);
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(64);
    ASSERT_EQ(bs.size(), 64);
    // always one buffer block
    ASSERT_EQ(bs.blocks(), 2);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 2);

    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
        ASSERT_EQ(bs.get(i), bs[i]);
        bs[i] = 1;
    }

    Bitset<block> bs_(65);
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
        ASSERT_EQ(bs.get(i), bs[i]);
    }

    for (uint32_t i = 0; i < bs.size(); i++) {
        bs_[i] = bs[i];
        ASSERT_EQ(bs_[i], bs[i]);
        ASSERT_EQ(bs_.get(i), bs[i]);
    }

    bs.clear();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
    }
    bs.set();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
    }

    bs.setBlock(0, 0);
    ASSERT_EQ(bs.getBlock(0), 0);

    for (uint32_t i = 0; i < bs.size() - 1; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[bs.size() - 1], 0);

    bs.setBlock(0, (block)-1);
    for (uint32_t i = 0; i < bs.size() - 1; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block)-1);
}

TEST(BitsetTest, BitsetFunctionality_UnsignedInt) {
    typedef uint32_t block;

    Bitset<block> bs;
    ASSERT_EQ(bs.size(), 0);
    // buffer block
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(1);
    ASSERT_EQ(bs.size(), 1);
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(64);
    ASSERT_EQ(bs.size(), 64);
    // always one buffer block
    ASSERT_EQ(bs.blocks(), 3);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 3);

    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
        ASSERT_EQ(bs.get(i), bs[i]);
        bs[i] = 1;
    }

    Bitset<block> bs_(65);
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
        ASSERT_EQ(bs.get(i), bs[i]);
    }

    for (uint32_t i = 0; i < bs.size(); i++) {
        bs_[i] = bs[i];
        ASSERT_EQ(bs_[i], bs[i]);
        ASSERT_EQ(bs_.get(i), bs[i]);
    }

    bs.clear();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
    }
    bs.set();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
    }

    bs.setBlock(0, 0);
    ASSERT_EQ(bs.getBlock(0), 0);
    for (uint32_t i = 0; i < 32; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[32], 0);

    bs.setBlock(0, (block)-1);
    for (uint32_t i = 0; i < 32; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block)-1);
}

TEST(BitsetTest, BitsetFunctionality_UnsignedShort) {
    typedef uint16_t block;

    Bitset<block> bs;
    ASSERT_EQ(bs.size(), 0);
    // buffer block
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(1);
    ASSERT_EQ(bs.size(), 1);
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(64);
    ASSERT_EQ(bs.size(), 64);
    // always one buffer block
    ASSERT_EQ(bs.blocks(), 5);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 5);

    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
        ASSERT_EQ(bs.get(i), bs[i]);
        bs[i] = 1;
    }

    Bitset<block> bs_(65);
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
        ASSERT_EQ(bs.get(i), bs[i]);
    }

    for (uint32_t i = 0; i < bs.size(); i++) {
        bs_[i] = bs[i];
        ASSERT_EQ(bs_[i], bs[i]);
        ASSERT_EQ(bs_.get(i), bs[i]);
    }

    bs.clear();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
    }
    bs.set();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
    }

    bs.setBlock(0, 0);
    ASSERT_EQ(bs.getBlock(0), 0);
    for (uint32_t i = 0; i < 16; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[16], 0);

    bs.setBlock(0, (block)-1);
    for (uint32_t i = 0; i < 16; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block)-1);
}

TEST(BitsetTest, BitsetFunctionality_UnsignedChar) {
    typedef uint8_t block;

    Bitset<block> bs;
    ASSERT_EQ(bs.size(), 0);
    // buffer block
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(1);
    ASSERT_EQ(bs.size(), 1);
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(64);
    ASSERT_EQ(bs.size(), 64);
    // always one buffer block
    ASSERT_EQ(bs.blocks(), 9);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 9);

    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
        ASSERT_EQ(bs.get(i), bs[i]);
        bs[i] = 1;
    }

    Bitset<block> bs_(65);
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
        ASSERT_EQ(bs.get(i), bs[i]);
    }

    for (uint32_t i = 0; i < bs.size(); i++) {
        bs_[i] = bs[i];
        ASSERT_EQ(bs_[i], bs[i]);
        ASSERT_EQ(bs_.get(i), bs[i]);
    }

    bs.clear();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
    }
    bs.set();
    for (uint32_t i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
    }

    bs.setBlock(0, 0);
    ASSERT_EQ(bs.getBlock(0), 0);
    for (uint32_t i = 0; i < 8; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[8], 0);

    bs.setBlock(0, (block)-1);
    for (uint32_t i = 0; i < 8; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block)-1);
}

TEST(BitsetTest, operators) {
    Bitset<uint64_t> b(100);
    Bitset<uint64_t> c(100);
    c.flip();
    for (uint64_t a = 0; a < c.size(); a++) {
        EXPECT_EQ(b[a], ~c[a]);
    }
    b.insert(10, 1);
    c &= b;
    for (uint64_t a = 0; a < c.size(); a++) {
        EXPECT_EQ(b[a], c[a]);
    }
    c ^= b;
    for (uint64_t a = 0; a < c.size(); a++) {
        EXPECT_EQ(c[a], 0);
    }
    b.insert(20, 1);
    c |= b;
    EXPECT_EQ(c[5], 0);
    EXPECT_EQ(c[10] & c[20], 1);
    c -= ~b;
    EXPECT_EQ(c[10] & c[20], 1);
    EXPECT_EQ(c[5], 0);
}
