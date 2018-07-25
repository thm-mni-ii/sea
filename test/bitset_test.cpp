#include <gtest/gtest.h>
#include <sealib/bitset.h>

using Sealib::Bitset;

TEST(BitsetTest, BitsetFunctionality_UnsignedLong) {
    typedef unsigned long block;

    Bitset<> bs;
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

TEST(BitsetTest, BitsetFunctionality_UnsignedInt) {
    typedef unsigned int block;

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
    ASSERT_EQ(bs.blocks(), 2);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 3);

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
    for (unsigned int i = 0; i < 32; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[32], 0);

    bs.setBlock(0, (block) - 1);
    for (unsigned int i = 0; i < 32; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block) - 1);
}

TEST(BitsetTest, BitsetFunctionality_UnsignedShort) {
    typedef unsigned short block;

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
    ASSERT_EQ(bs.blocks(), 4);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 5);

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
    for (unsigned int i = 0; i < 16; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[16], 0);

    bs.setBlock(0, (block) - 1);
    for (unsigned int i = 0; i < 16; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block) - 1);
}

TEST(BitsetTest, BitsetFunctionality_UnsignedChar) {
    typedef unsigned char block;

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
    ASSERT_EQ(bs.blocks(), 8);

    bs = Bitset<block>(65);
    ASSERT_EQ(bs.size(), 65);
    ASSERT_EQ(bs.blocks(), 9);

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
    for (unsigned int i = 0; i < 8; i++) {
        ASSERT_EQ(bs[i], 0);
        bs[i].flip();
        ASSERT_EQ(bs[i], 1);
        bs[i].flip();
        ASSERT_EQ(bs[i], 0);
    }
    ASSERT_EQ(bs[bs.size() - 1], 1);

    bs.setBlock(1, 0);
    ASSERT_EQ(bs[8], 0);

    bs.setBlock(0, (block) - 1);
    for (unsigned int i = 0; i < 8; i++) {
        ASSERT_EQ(bs[i], 1);
    }

    ASSERT_EQ(bs.getBlock(0), (block) - 1);
}
