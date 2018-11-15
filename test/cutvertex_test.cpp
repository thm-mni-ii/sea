#include <gtest/gtest.h>
#include <iostream>
#include "sealib/cutvertexiterator.h"
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(CutVertexIteratorTest, windmillGraph) {
    BasicGraph *g = GraphCreator::createWindmill(3, 4);
    CutVertexIterator c(g);
    c.init();

    // there should be exactly one cut vertex (the windmill's center, which has
    // the last index in G)
    EXPECT_TRUE(c.more());
    EXPECT_EQ(c.next(), g->getOrder() - 1);
    EXPECT_FALSE(c.more());
}
