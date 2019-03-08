#include "sealib/collection/avltree.h"
#include <gtest/gtest.h>

using namespace Sealib;  // NOLINT

TEST(AVLTreeTest, swapLeaves) {
    AVLTree t;
    t.insert(6, 12);
    t.insert(4, 7);
    t.insert(5, 40);
    EXPECT_EQ(t.search(12), INVALID);
    EXPECT_EQ(t.search(4), 7);
    EXPECT_EQ(t.search(6), 12);
    EXPECT_EQ(t.search(5), 40);
    t.insert(6, 25);
    EXPECT_EQ(t.search(25), INVALID);
    EXPECT_EQ(t.search(6), 25);
}

TEST(AVLTree, rotateTree) {
    AVLTree t;
    t.insert(10,1);
    t.insert(8,2);
    t.insert(12,0);
    t.insert(6,0);
    t.insert(9,0);
    t.insert(4,3);
    EXPECT_EQ(t.search(3),INVALID);
    EXPECT_EQ(t.search(6),0);
    EXPECT_EQ(t.search(9),0);
}
