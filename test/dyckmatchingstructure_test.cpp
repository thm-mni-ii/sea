#include <gtest/gtest.h>
#include <sealib/dyckmatchingstructure.h>
#include <sealib/dyckwordlexicon.h>

TEST(DyckMatchingStructureTest, testAllWordsLength10) {
    Sealib::DyckWordLexicon lex(10);
    for (const Sealib::Bitset<uint8_t> &word : lex.getLexicon()) {
        Sealib::DyckMatchingStructure d(word);
        for (uint32_t i = 0; i < 10; i++) {
            ASSERT_NE(d.getMatch(i), i);
        }
    }
}

TEST(DyckMatchingStructureTest, testWords) {
    Sealib::Bitset<uint8_t> word1(2);
    word1[0] = 1;
    Sealib::DyckMatchingStructure d1(word1);

    ASSERT_EQ(d1.getMatch(0), 1);
    ASSERT_EQ(d1.getMatch(1), 0);

    Sealib::Bitset<uint8_t> word2(10);
    for (uint32_t i = 0; i < 5; i++) {
        word2[i] = 1;
    }
    Sealib::DyckMatchingStructure d2(word2);

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
}
