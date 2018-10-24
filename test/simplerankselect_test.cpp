#include <gtest/gtest.h>
#include <sealib/simplerankselect.h>
#include <sealib/sharedrankselect.h>

TEST(SimpleRankSelectTest, simpleRankSelect) {
    std::shared_ptr<Sealib::Bitset<unsigned char>> bits(new Sealib::Bitset<unsigned char>(1));
    (*bits)[0] = 1;

    Sealib::SimpleRankSelect simpleRankSelect(bits);
    ASSERT_EQ(1, simpleRankSelect.select(1));
    ASSERT_EQ(1, simpleRankSelect.rank(1));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(2));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(2));

    (*bits)[0] = 0;
    simpleRankSelect = Sealib::SimpleRankSelect(bits);
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(1));
    ASSERT_EQ(0, simpleRankSelect.rank(1));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(2));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(2));
}
