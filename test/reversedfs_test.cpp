#include "sealib/iterator/reversedfs.h"
#include <gtest/gtest.h>
#include <cstdio>
#include "../src/dfs/simplereversedfs.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/graphio.h"

using namespace Sealib;  // NOLINT

static uint64_t GRAPHCOUNT = 8, ORDER = 2500, DEGREE = 16;

static std::vector<DirectedGraph> makeGraphs() {
    std::vector<DirectedGraph> g;
    for (uint64_t c = 0; c < GRAPHCOUNT; c++) {
        g.push_back(Sealib::GraphCreator::kOutdegree(ORDER, DEGREE));
    }
    return g;
}

class ReverseDFSTest : public ::testing::TestWithParam<DirectedGraph> {};

INSTANTIATE_TEST_CASE_P(ParamTests, ReverseDFSTest,
                        ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(ReverseDFSTest, comparison) {
    DirectedGraph const& g = GetParam();
    ReverseDFS r(g);
    SimpleReverseDFS s(g);
    r.init();
    s.init();
    std::vector<UserCall> v1, v2;
    while (r.more()) {
        UserCall a = r.next();
        v1.push_back(a);
    }
    while (s.more()) {
        UserCall a = s.next();
        v2.push_back(a);
    }
    EXPECT_EQ(v1.size(), v2.size());
    bool equal = true;
    uint64_t n = v1.size() <= v2.size() ? v1.size() : v2.size();
    // printf("[ ");
    for (uint64_t a = 0; a < n; a++) {
        if (v1[a].u != v1[a].u) {
            equal = false;
            // printf(" <<%lu!=%lu>> ", v1[a].u, v2[a].u);
        } else {
            // printf(" %lu ", v1[a].u);
        }
    }
    // printf(" ]\n");
    EXPECT_TRUE(equal);
}
