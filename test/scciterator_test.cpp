#include "sealib/iterator/scciterator.h"
#include <gtest/gtest.h>
#include <vector>
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(SCCIteratorTest, noDuplicates) {
    DirectedGraph g = GraphCreator::kOutdegree(50, 2);
    SCCIterator s(g);
    s.init();
    s.more();
    s.next();
    SUCCEED();
}
