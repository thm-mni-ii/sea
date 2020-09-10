#include <gtest/gtest.h>
#include <sealib/dictionary/rankselect.h>

#include <bitset>
#include <sealib/dictionary/rank9.hpp>
#include <sealib/dictionary/rank9select.hpp>

#include "../src/rankselect/simplerankselect.h"

using namespace Sealib;  // NOLINT

TEST(RankSelectTest, rankSelect) {
    std::shared_ptr<Bitset<uint8_t>> bits_(new Bitset<uint8_t>(1));
    (*bits_)[0] = true;

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

                Bitset<uint64_t > r9bits(16+e);

                auto bc1 = std::bitset<8>(c1);
                auto bc2 = std::bitset<8>(c2);
                for (auto i = 0; i < 8; i++) {
                    bits->insert(i, bc1[i]);
                    bits->insert(i+8, bc2[i]);
                    r9bits.insert(i, bc1[i]);
                    r9bits.insert(i+8, bc2[i]);
                }
                for (size_t i = 0; i < e; i++) {
                    bits->insert(16 + i, true);
                    r9bits.insert(16 + i, true);
                }

                SimpleRankSelect simpleRankSelect(bits);
                RankSelect rankSelect(*bits);
                Rank9Select rank9Select(r9bits);
                for (uint32_t i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                    ASSERT_EQ(rank9Select.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rank9Select.rank(i), simpleRankSelect.rank(i));
                }
            }
            for (uint32_t e = 0; e < 8; e++) {
                std::shared_ptr<Bitset<uint8_t>> bits(
                    new Bitset<uint8_t>(16 + e));

                Bitset<uint64_t > r9bits(16+e);

                auto bc1 = std::bitset<8>(c1);
                auto bc2 = std::bitset<8>(c2);
                for (size_t i = 0; i < 8; i++) {
                    bits->insert(i, bc1[i]);
                    bits->insert(i+8, bc2[i]);
                    r9bits.insert(i, bc1[i]);
                    r9bits.insert(i+8, bc2[i]);
                }
                for (size_t i = 0; i < e; i++) {
                    bits->insert(16 + i, true);
                    r9bits.insert(16 + i, true);
                }

                SimpleRankSelect simpleRankSelect(bits);
                RankSelect rankSelect(*bits);
                Rank9Select rank9Select(r9bits);
                for (uint32_t i = 0; i <= bits->size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                    ASSERT_EQ(rank9Select.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rank9Select.rank(i), simpleRankSelect.rank(i));
                }
            }
        } while (++c2);
    } while (++c1);
}

TEST(Rank9SelectTest, rank9select) {
    Bitset<uint64_t> bits_(1);
    bits_[0] = true;

    Rank9Select rs(bits_);
    ASSERT_EQ(1, rs.select(1));
    ASSERT_EQ(1, rs.rank(1));
    ASSERT_EQ(INVALID, rs.select(0));
    ASSERT_EQ(INVALID, rs.rank(0));
    ASSERT_EQ(INVALID, rs.select(2));
    ASSERT_EQ(INVALID, rs.rank(2));
}
