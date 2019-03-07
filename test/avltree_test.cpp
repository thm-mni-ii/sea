#include "sealib/collection/avltree.h"
#include <gtest/gtest.h>

using namespace Sealib;  // NOLINT

TEST(AVLTreeTest, search) {
    AVLTree t;
    std::vector<uint64_t> board = {7, 12, 40};
    t.insert(6, 12);
    t.insert(4, 7);
    t.insert(3, 40);
    EXPECT_EQ(t.search(5), INVALID);
    EXPECT_EQ(t.search(4), 7);
    EXPECT_EQ(t.search(6), 12);
    t.insert(6, 25);
    EXPECT_EQ(t.search(95), INVALID);
    EXPECT_EQ(t.search(6), 25);
    EXPECT_EQ(t.search(3), 40);
}
