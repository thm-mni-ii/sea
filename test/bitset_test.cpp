#include <gtest/gtest.h>
#include <sealib/bitset.h>

using Sealib::Bitset;

TEST(BitsetTest, BitsetFunctionality) {
    typedef unsigned long block;

    Bitset<block> bs;
    std::cout << bs.size() << std::endl;
    std::cout << bs.blocks() << std::endl;
    ASSERT_EQ(bs.size(), 0);
    ASSERT_EQ(bs.blocks(), 0);

    bs = Bitset<block> (1);
    ASSERT_EQ(bs.size(), 1);
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(64);
    ASSERT_EQ(bs.size(), 64);
    ASSERT_EQ(bs.blocks(), 1);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 2);

    for (unsigned int i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
        ASSERT_EQ(bs.get(i), bs[i]);
        bs[i] = 1;
    }

    Bitset<block> bs_(65);
    for (unsigned int i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
        ASSERT_EQ(bs.get(i), bs[i]);
    }

    for (unsigned int i = 0; i < bs.size(); i++) {
        bs_[i] = bs[i];
        ASSERT_EQ(bs_[i], bs[i]);
        ASSERT_EQ(bs_.get(i), bs[i]);
    }

    bs.clear();
    for (unsigned int i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 0);
    }
    bs.set();
    for (unsigned int i = 0; i < bs.size(); i++) {
        ASSERT_EQ(bs[i], 1);
    }

    bs.setBlock(0, 0);
    ASSERT_EQ(bs.getBlock(0), 0);

    for (unsigned int i = 0; i < bs.size() - 1; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[bs.size() - 1], 0);

    bs.setBlock(0, (block) - 1);
    for (unsigned int i = 0; i < bs.size() - 1; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block) - 1);
}
