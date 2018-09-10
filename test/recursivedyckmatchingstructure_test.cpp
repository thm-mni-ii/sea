#include <gtest/gtest.h>
#include <include/sealib/recursivedyckmatchingstructure.h>
#include <include/sealib/dyckwordlexicon.h>

/*TEST(RecursiveDyckMatchingStructureTest, testAllWordsLength10) {
    Sealib::DyckWordLexicon lex(10);
    for (const Sealib::Bitset<unsigned char> &word : lex.getLexicon()) {
        Sealib::RecursiveDyckMatchingStructure d(word);
        for (unsigned int i = 0; i < 10; i++) {
            ASSERT_NE(d.getMatch(i), i);
        }
    }
}*/

TEST(RecursiveDyckMatchingStructureTest, testWords) {
    Sealib::Bitset<unsigned char> word(16);
    word.setBlock(0, (unsigned char) -1);

    Sealib::Bitset<unsigned char> segmentizedWord =
        Sealib::DyckMatchingStructure::segmentizeWord(word);

    Sealib::DyckMatchingStructure::getMatchNaive(segmentizedWord, 8, 7);

    for (unsigned int i = 0; i < 16; i++) {
        unsigned long match =
            Sealib::DyckMatchingStructure::getMatchNaive(word, i);
        unsigned long segMatch =
            Sealib::DyckMatchingStructure::getMatchNaive(segmentizedWord, i, 7);
        if (match != segMatch) {
            std::cout << "match " << i << ": " << match << std::endl;
            std::cout << "seg match " << i << ": " << segMatch << std::endl;
        }
    }

    Sealib::Bitset<unsigned char> word1(2);
    word1[0] = 1;
    Sealib::RecursiveDyckMatchingStructure d1(word1);

    ASSERT_EQ(d1.getMatch(0), 1);
    ASSERT_EQ(d1.getMatch(1), 0);

    Sealib::Bitset<unsigned char> word2(10);
    for (unsigned int i = 0; i < 5; i++) {
        word2[i] = 1;
    }
    Sealib::RecursiveDyckMatchingStructure d2(word2);

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
