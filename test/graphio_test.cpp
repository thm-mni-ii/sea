#include "sealib/graph/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(GraphIOTest, exportGML) {
    DirectedGraph g = GraphCreator::createRandomFixed(10, 5);
    GraphExporter::exportGML(&g, "graphio1.gml");
}

TEST(GraphIOTest, importGML) {
    DirectedGraph g = GraphImporter::importGML<DirectedGraph>("graphio1.gml");
    EXPECT_EQ(g.getOrder(), 10);
    for (uint u = 0; u < g.getOrder(); u++) {
        EXPECT_EQ(g.getNodeDegree(u), 5);
    }
}
