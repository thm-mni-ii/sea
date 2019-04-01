#include <gtest/gtest.h>
#include "../src/rankselect/simplerankselect.h"
#include <sealib/dictionary/rankselect.h>

using namespace Sealib;  // NOLINT

TEST(RankSelectTest, rankSelect) {
    std::shared_ptr<Bitset<uint8_t>> bits_(new Bitset<uint8_t>(1));
    (*bits_)[0] = 1;

    RankSelect rs(*bits_);
    ASSERT_EQ(1, rs.select(1));
    ASSERT_EQ(1, rs.rank(1));
    ASSERT_EQ(INVALID, rs.select(0));
    ASSERT_EQ(INVALID, rs.rank(0));
    ASSERT_EQ(INVALID, rs.select(2));
    ASSERT_EQ(INVALID, rs.rank(2));

    uint8_t c1 = 0;
    do {
        uint8_t c2 = 0;
        do {
            for (uint32_t e = 0; e < 8; e++) {
                std::shared_ptr<Bitset<uint8_t>> bits(
                    new Bitset<uint8_t>(16 + e));
                bits->setBlock(0, c1);
                bits->setBlock(1, c2);
                for (uint32_t i = 0; i < e; i++) {
                    (*bits)[16 + i] = 1;
                }

                SimpleRankSelect simpleRankSelect(bits);
                RankSelect rankSelect(*bits);
                for (uint32_t i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
            for (uint32_t e = 0; e < 8; e++) {
                std::shared_ptr<Bitset<uint8_t>> bits(
                    new Bitset<uint8_t>(16 + e));
                bits->setBlock(0, c1);
                bits->setBlock(1, c2);
                for (uint32_t i = 0; i < e; i++) {
                    (*bits)[16 + i] = 0;
                }
                SimpleRankSelect simpleRankSelect(bits);
                RankSelect rankSelect(*bits);
                for (uint32_t i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
        } while (++c2);
    } while (++c1);
}
