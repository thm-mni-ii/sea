#include <gtest/gtest.h>
#include <include/sealib/simplerankselect.h>
#include <include/sealib/sharedrankselect.h>

TEST(SharedRankSelectTest, rankSelect) {
    std::shared_ptr<Sealib::Bitset<unsigned char>> bits_(new Sealib::Bitset<unsigned char>(1));
    (*bits_)[0] = 1;

    Sealib::SharedRankSelect rs(bits_);
    ASSERT_EQ(1, rs.select(1));
    ASSERT_EQ(1, rs.rank(1));
    ASSERT_EQ((unsigned long) -1, rs.select(0));
    ASSERT_EQ((unsigned long) -1, rs.rank(0));
    ASSERT_EQ((unsigned long) -1, rs.select(2));
    ASSERT_EQ((unsigned long) -1, rs.rank(2));

    unsigned char c1 = 0;
    do {
        unsigned char c2 = 0;
        do {
            for (unsigned int e = 0; e < 8; e++) {
                std::shared_ptr<Sealib::Bitset<unsigned char>> bits(
                    new Sealib::Bitset<unsigned char>(16 + e));
                bits->setBlock(0, c1);
                bits->setBlock(1, c2);
                for (unsigned int i = 0; i < e; i++) {
                    (*bits)[16 + i] = 1;
                }

                Sealib::SimpleRankSelect simpleRankSelect(bits);
                Sealib::SharedRankSelect rankSelect(bits);
                for (unsigned int i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
            for (unsigned int e = 0; e < 8; e++) {
                std::shared_ptr<Sealib::Bitset<unsigned char>> bits(
                    new Sealib::Bitset<unsigned char>(16 + e));
                bits->setBlock(0, c1);
                bits->setBlock(1, c2);
                for (unsigned int i = 0; i < e; i++) {
                    (*bits)[16 + i] = 0;
                }
                Sealib::SimpleRankSelect simpleRankSelect(bits);
                Sealib::SharedRankSelect rankSelect(bits);
                for (unsigned int i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
        } while (++c2);
    } while (++c1);
}
