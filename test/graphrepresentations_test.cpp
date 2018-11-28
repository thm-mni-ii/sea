#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/graph/compactgraph.h>
#include <sealib/graph/graphrepresentations.h>

using Sealib::Graphrepresentations;

TEST(CompactGraphRepresentationsTest, beginpointer) {
    uint *A = new uint[16] {5,  7, 9, 11, 13, 15,  9,  2, 4,  3, 4,  1, 2,  2, 3,  3};

    Graphrepresentations::standardToBeginpointer(A);

    ASSERT_EQ(A[0], 5);

    ASSERT_EQ(A[7], 9);
    ASSERT_EQ(A[9], 11);
    ASSERT_EQ(A[11], 7);
    ASSERT_EQ(A[13], 9);
    ASSERT_EQ(A[15], 11);
}

TEST(CompactGraphRepresentationsTest, swapped) {
    uint *A = new uint[16] {5,  7, 9, 11, 13, 15,  9,  2, 4,  3, 4,  1, 2,  2, 3,  3};

    Graphrepresentations::standardToBeginpointer(A);
    Graphrepresentations::swapRepresentation(A);

    ASSERT_EQ(A[0], 5);

    ASSERT_EQ(A[7], 1);
    ASSERT_EQ(A[9], 2);
    ASSERT_EQ(A[11], 3);
    ASSERT_EQ(A[13], 4);
    ASSERT_EQ(A[15], 5);

    ASSERT_EQ(A[1], 9);
    ASSERT_EQ(A[2], 11);
    ASSERT_EQ(A[3], 7);
    ASSERT_EQ(A[4], 9);
    ASSERT_EQ(A[5], 11);

    Graphrepresentations::swappedBeginpointerToStandard(A);

    ASSERT_EQ(A[0], 5);
    ASSERT_EQ(A[1], 7);
    ASSERT_EQ(A[6], 9);
    ASSERT_EQ(A[7], 2);
    ASSERT_EQ(A[9], 3);
    ASSERT_EQ(A[11], 1);
    ASSERT_EQ(A[13], 2);
    ASSERT_EQ(A[15], 3);
}
