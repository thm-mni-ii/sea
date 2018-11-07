#include "sealib/cutvertex.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(CutVertexIteratorTest, init) {
    std::pair<BasicGraph *, uint> p =
        GraphCreator::createRandomUndirected(100, 3);
    CutVertexIterator c(p.first);
    c.init();
}
