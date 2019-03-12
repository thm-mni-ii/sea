#include "sealib/iterator/scciterator.h"
#include <gtest/gtest.h>
#include <vector>
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(SCCIteratorTest, DISABLED_noDuplicates) {
    DirectedGraph g = GraphCreator::kOutdegree(50, 2);
    std::vector<uint64_t> v;
    SCCIterator s(g, [&v](uint64_t u) { v.push_back(u); });
    s.init();
    while (s.more()) s.next();
    EXPECT_EQ(g.getOrder(), v.size());
}
