#include "sealib/dictionary/raggeddictionary.h"
#include <gtest/gtest.h>

using namespace Sealib;  // NOLINT

TEST(RaggedDictionaryTest, allOperations) {
    RaggedDictionary d(128);
    d.insert(0, 17);
    d.insert(127, 500);
    EXPECT_EQ(d.get(0), 17);
    EXPECT_EQ(d.get(1), INVALID);
    EXPECT_EQ(d.get(127), 500);
    EXPECT_TRUE(d.member(127));
    EXPECT_FALSE(d.member(126));

    uint64_t id = d.someId();
    EXPECT_TRUE(id == 0 || id == 127);
    ChoiceDictionaryIterator c = d.allIds();
    EXPECT_TRUE(c.more());
    EXPECT_EQ(c.next(), 0);
    EXPECT_TRUE(c.more());
    EXPECT_EQ(c.next(), 127);
    EXPECT_FALSE(c.more());
}

TEST(RaggedDictionaryTest, limit) {
    RaggedDictionary d(512);
    // log2(512)=9 => max. keys: 512/9=56
    for (uint64_t a = 0; a < 56; a++) {
        d.insert(a, 0);
    }
    EXPECT_THROW(d.insert(56, 0), RaggedDictionaryFull);
}
