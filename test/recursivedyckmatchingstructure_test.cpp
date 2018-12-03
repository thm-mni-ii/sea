#include <gtest/gtest.h>
#include <sealib/recursivedyckmatchingstructure.h>
#include <sealib/dyckwordlexicon.h>

TEST(RecursiveDyckMatchingStructureTest, testAllWordsLength10) {
    for (uint32_t i = 2; i < 23; i += 2) {
        Sealib::DyckWordLexicon lex(i);
        for (const Sealib::Bitset<uint8_t> &word : lex.getLexicon()) {
            Sealib::DyckMatchingStructure d(word);
            Sealib::RecursiveDyckMatchingStructure dRec(word);
            for (uint32_t j = 0; j < i; j++) {
                uint64_t dMatch = d.getMatch(j);
                uint64_t dRecMatch = dRec.getMatch(j);
                ASSERT_NE(dMatch, j);
                ASSERT_NE(dRecMatch, j);
                ASSERT_EQ(dMatch, dRecMatch);
            }
        }
    }
}

TEST(RecursiveDyckMatchingStructureTest, testWords) {
    Sealib::Bitset<uint8_t> word(16);
    word.setBlock(0, (uint8_t) -1);

    Sealib::Bitset<uint8_t> word1(2);
    word1[0] = 1;
    Sealib::RecursiveDyckMatchingStructure d1(word1);

    ASSERT_EQ(d1.getMatch(0), 1);
    ASSERT_EQ(d1.getMatch(1), 0);

    Sealib::Bitset<uint8_t> word2(10);
    for (uint32_t i = 0; i < 5; i++) {
        word2[i] = 1;
    }
    Sealib::RecursiveDyckMatchingStructure d2(word2);
    Sealib::DyckMatchingStructure d2simple(word2);

    ASSERT_EQ(d2.getMatch(0), 9);
    ASSERT_EQ(d2.getMatch(1), 8);
    ASSERT_EQ(d2.getMatch(2), 7);
    ASSERT_EQ(d2.getMatch(3), 6);
    ASSERT_EQ(d2.getMatch(4), 5);

    ASSERT_EQ(d2.getMatch(9), 0);
    ASSERT_EQ(d2.getMatch(8), 1);
    ASSERT_EQ(d2.getMatch(7), 2);
    ASSERT_EQ(d2.getMatch(6), 3);
    ASSERT_EQ(d2.getMatch(5), 4);

    ASSERT_EQ(d2.getMatch(0), d2simple.getMatch(0));
    ASSERT_EQ(d2.getMatch(1), d2simple.getMatch(1));
    ASSERT_EQ(d2.getMatch(2), d2simple.getMatch(2));
    ASSERT_EQ(d2.getMatch(3), d2simple.getMatch(3));
    ASSERT_EQ(d2.getMatch(4), d2simple.getMatch(4));

    ASSERT_EQ(d2.getMatch(5), d2simple.getMatch(5));
    ASSERT_EQ(d2.getMatch(6), d2simple.getMatch(6));
    ASSERT_EQ(d2.getMatch(7), d2simple.getMatch(7));
    ASSERT_EQ(d2.getMatch(8), d2simple.getMatch(8));
    ASSERT_EQ(d2.getMatch(9), d2simple.getMatch(9));
}
