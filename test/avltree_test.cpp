#include "sealib/collection/avltree.h"
#include <gtest/gtest.h>

using namespace Sealib;  // NOLINT

TEST(AVLTree, basicInsert) {
    AVLTree t;
    t.insert(5, 1);
    t.insert(0, 2);
    t.insert(10, 3);
    EXPECT_EQ(t.search(5), 1);
    EXPECT_EQ(t.search(0), 2);
    EXPECT_EQ(t.search(10), 3);
    t.insert(0, 5);
    EXPECT_EQ(t.search(0), 5);
}

TEST(AVLTreeTest, swapLeavesR) {
    AVLTree t;
    t.insert(6, 1);
    t.insert(4, 2);
    t.insert(5, 3);
    EXPECT_EQ(t.search(5), 3);
    EXPECT_EQ(t.search(4), 2);
    EXPECT_EQ(t.search(6), 1);
}

TEST(AVLTree, swapLeavesL) {
    AVLTree t;
    t.insert(4, 1);
    t.insert(6, 2);
    t.insert(5, 3);
    EXPECT_EQ(t.search(5), 3);
    EXPECT_EQ(t.search(6), 2);
    EXPECT_EQ(t.search(4), 1);
}

TEST(AVLTree, rotateTreeR) {
    AVLTree t;
    t.insert(10, 1);
    t.insert(8, 2);
    t.insert(12, 0);
    t.insert(6, 0);
    t.insert(9, 0);
    t.insert(4, 3);
    EXPECT_EQ(t.search(6), 0);
    EXPECT_EQ(t.search(9), 0);
}

TEST(AVLTree, rotateTreeL) {
    AVLTree t;
    t.insert(10, 1);
    t.insert(12, 2);
    t.insert(8, 0);
    t.insert(11, 0);
    t.insert(14, 0);
    t.insert(16, 3);
    EXPECT_EQ(t.search(11), 0);
    EXPECT_EQ(t.search(14), 0);
}
