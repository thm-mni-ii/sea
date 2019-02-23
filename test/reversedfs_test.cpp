#include "sealib/iterator/reversedfs.h"
#include <gtest/gtest.h>
#include <cstdio>
#include "../src/dfs/simplereversedfs.h"
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(ReverseDFSTest, postprocess) {
    DirectedGraph g = GraphCreator::kOutdegree(6, 1);
    ReverseDFS r(g);
    SimpleReverseDFS s(g, UserCall::postprocess);
    r.init();
    s.init();
    std::vector<UserCall> v1, v2;
    while(r.more()) {
        UserCall a=r.next();
        if(a.type==UserCall::postprocess) v1.push_back(a);
    }
    while(s.more()) {
        UserCall a=s.next();
        if(a.type==UserCall::postprocess) v2.push_back(a);
    }
    ASSERT_GE(v2.size(),v1.size());
    bool equal=true;
    printf("[ ");
    for(uint64_t a=0; a<v1.size(); a++) {
        if(v1[a].u!=v2[a].u) {
            equal=false;
            printf(" <<%lu!=%lu>> ",v1[a].u,v2[a].u);
        }
        else {
            printf(" %lu ",v1[a].u);
        }
    }
    printf(" ]\n");
    EXPECT_TRUE(equal);
}
