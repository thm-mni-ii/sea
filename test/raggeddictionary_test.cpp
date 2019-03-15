#include "sealib/dictionary/raggeddictionary.h"
#include <gtest/gtest.h>

using namespace Sealib;  // NOLINT

TEST(RaggedDictionaryTest, basic) {
    RaggedDictionary d(128);
    d.insert(0, 17);
    d.insert(127, 500);
    EXPECT_EQ(d.get(0), 17);
    EXPECT_EQ(d.get(1), INVALID);
    EXPECT_EQ(d.get(127), 500);
}