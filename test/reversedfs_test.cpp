#include "sealib/iterator/reversedfs.h"
#include <gtest/gtest.h>
#include "../src/dfs/simplereversedfs.h"
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(ReverseDFSTest, postprocess) {
    DirectedGraph g = GraphCreator::kOutdegree(1024, 16);
    ReverseDFS r(g);
    SimpleReverseDFS s(g, UserCall::postprocess);
    r.init();
    s.init();
    while (r.more()) {
        UserCall a = r.next(), b;
        if (a.type == UserCall::postprocess) {
            b = s.next();
            EXPECT_EQ(a, b);
        }
    }
    EXPECT_FALSE(r.more());
    EXPECT_FALSE(s.more());
}
