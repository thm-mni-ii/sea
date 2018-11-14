#include "sealib/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(GraphIOTest, exportGML) {
    Graph *g = GraphCreator::createRandomFixed(10, 5);
    // Graph *g=GraphCreator::createWindmill(3,4);
    GraphExporter::exportGML(g, "graphio1.gml");
}

TEST(GraphIOTest, importGML) {
    BasicGraph g = GraphImporter::importGML<BasicGraph>("graphio1.gml");
    EXPECT_EQ(g.getOrder(), 10);
    for (uint u = 0; u < g.getOrder(); u++) {
        EXPECT_EQ(g.getNodeDegree(u), 5);
    }
}
