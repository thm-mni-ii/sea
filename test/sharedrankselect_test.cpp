#include <gtest/gtest.h>
#include <sealib/simplerankselect.h>
#include <sealib/sharedrankselect.h>

TEST(SharedRankSelectTest, rankSelectChar) {
    std::shared_ptr<Sealib::Bitset<unsigned char>> bits_(new Sealib::Bitset<unsigned char>(1));
    (*bits_)[0] = 1;

    Sealib::SharedRankSelect<> rs(bits_);
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

                Sealib::SimpleRankSelect<> simpleRankSelect(bits);
                Sealib::SharedRankSelect<> rankSelect(bits);
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
                Sealib::SimpleRankSelect<> simpleRankSelect(bits);
                Sealib::SharedRankSelect<> rankSelect(bits);
                for (unsigned int i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
        } while (++c2);
    } while (++c1);
}

TEST(SharedRankSelectTest, rankSelectShort) {
    std::shared_ptr<Sealib::Bitset<unsigned short>> bits_(new Sealib::Bitset<unsigned short>(1));
    (*bits_)[0] = 1;

    Sealib::SharedRankSelect<unsigned short> rs(bits_);
    ASSERT_EQ(1, rs.select(1));
    ASSERT_EQ(1, rs.rank(1));
    ASSERT_EQ((unsigned long) -1, rs.select(0));
    ASSERT_EQ((unsigned long) -1, rs.rank(0));
    ASSERT_EQ((unsigned long) -1, rs.select(2));
    ASSERT_EQ((unsigned long) -1, rs.rank(2));

    unsigned short c1 = 0;
    do {
        for (unsigned int e = 0; e < 8; e++) {
            std::shared_ptr<Sealib::Bitset<unsigned short>> bits(
                new Sealib::Bitset<unsigned short>(16 + e));
            bits->setBlock(0, c1);
            for (unsigned int i = 0; i < e; i++) {
                (*bits)[16 + i] = 1;
            }

            Sealib::SimpleRankSelect<unsigned short> simpleRankSelect(bits);
            Sealib::SharedRankSelect<unsigned short> rankSelect(bits);
            for (unsigned int i = 0; i <= bits->size(); i++) {
                ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
            }
        }
        for (unsigned int e = 0; e < 8; e++) {
            std::shared_ptr<Sealib::Bitset<unsigned short>> bits(
                new Sealib::Bitset<unsigned short>(16 + e));
            bits->setBlock(0, c1);
            for (unsigned int i = 0; i < e; i++) {
                (*bits)[16 + i] = 0;
            }
            Sealib::SimpleRankSelect<unsigned short> simpleRankSelect(bits);
            Sealib::SharedRankSelect<unsigned short> rankSelect(bits);
            for (unsigned int i = 0; i <= bits->size(); i++) {
                ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
            }
        }
    } while (++c1);
}