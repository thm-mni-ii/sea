#include "sealib/bcciterator.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(BCCIteratorTest, windmillGraph) {
    BasicGraph *g = GraphCreator::createWindmill(3, 4);
    BCCIterator b(g);
    b.init();
    b.start(1, 8);
    while (b.more()) {
        Pair n = b.next();
        std::cout << n.head() << "," << n.tail() << " ";
    }
    // EXPECT_TRUE(b.more());
    // EXPECT_EQ(b.next(),1);
    // EXPECT_TRUE(b.more());
}
