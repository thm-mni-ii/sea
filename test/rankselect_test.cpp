//
// Created by jmeintrup on 29.05.18.
//
#include <gtest/gtest.h>
#include <include/sealib/simplerankselect.h>
#include <include/sealib/rankselect.h>

TEST(RankSelectTest, simpleRankSelect1) {
    boost::dynamic_bitset<unsigned char> bits(1);
    bits[0] = 1;

    Sealib::SimpleRankSelect simpleRankSelect(bits);
    ASSERT_EQ(1, simpleRankSelect.select(1));
    ASSERT_EQ(1, simpleRankSelect.rank(1));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(2));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(2));

    bits[0] = 0;
    simpleRankSelect = Sealib::SimpleRankSelect(bits);
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(1));
    ASSERT_EQ(0, simpleRankSelect.rank(1));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(0));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.select(2));
    ASSERT_EQ((unsigned long) - 1, simpleRankSelect.rank(2));
}

TEST(RankSelectTest, rankSelect) {
    unsigned char c1 = 0 ;
    do {
        unsigned char c2 = 0 ;
        do {
            boost::dynamic_bitset<unsigned char> bits;
            bits.append(c1);
            bits.append(c2);
            for(unsigned int e = 0; e < 8; e++) {
                bits.push_back(1);
                Sealib::SimpleRankSelect simpleRankSelect(bits);
                Sealib::RankSelect rankSelect(bits);
                for(unsigned int i = 0; i <= bits.size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
            bits = boost::dynamic_bitset<unsigned char>();
            bits.append(c1);
            bits.append(c2);
            for(unsigned int e = 0; e < 8; e++) {
                bits.push_back(0);
                Sealib::SimpleRankSelect simpleRankSelect(bits);
                Sealib::RankSelect rankSelect(bits);
                for(unsigned int i = 0; i <= bits.size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
            bits = boost::dynamic_bitset<unsigned char>();
            bits.append(c1);
            bits.append(c2);
            for(unsigned int e = 0; e < 3; e++) {
                bits.push_back(0);
                bits.push_back(1);
                Sealib::SimpleRankSelect simpleRankSelect(bits);
                Sealib::RankSelect rankSelect(bits);
                for(unsigned int i = 0; i <= bits.size(); i++) {
                    ASSERT_EQ(rankSelect.select(i), simpleRankSelect.select(i));
                    ASSERT_EQ(rankSelect.rank(i), simpleRankSelect.rank(i));
                }
            }
        }
        while ( ++c2 );
    }
    while ( ++c1 );
}