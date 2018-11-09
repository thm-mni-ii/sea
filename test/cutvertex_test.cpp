#include <gtest/gtest.h>
#include <iostream>
#include "sealib/cutvertexiterator.h"
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(CutVertexIteratorTest, init) {
    std::pair<BasicGraph *, uint> p =
        GraphCreator::createRandomUndirected(100, 10);
    CutVertexIterator c(p.first);
    c.init();
    while (c.more()) std::cout << c.next() << " ";
}
