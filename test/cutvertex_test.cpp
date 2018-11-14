#include <gtest/gtest.h>
#include <iostream>
#include "sealib/cutvertexiterator.h"
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(CutVertexIteratorTest, init) {
    BasicGraph *g = GraphCreator::createWindmill(3, 4);
    CutVertexIterator c(g);
    c.init();
    while (c.more()) std::cout << c.next() << " ";
}
