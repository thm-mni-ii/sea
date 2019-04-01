#include <gtest/gtest.h>
#include "../src/collection/largedoublelinkedlist.h"

using namespace Sealib;  // NOLINT

TEST(DoubleLinkedListTest, basic) {
    LargeDoubleLinkedList l(10);
    EXPECT_FALSE(l.isEmpty());
    EXPECT_EQ(l.get(), 0);
    EXPECT_EQ(l.get(), 1);
    l.remove(2);
    EXPECT_EQ(l.get(), 3);
}
