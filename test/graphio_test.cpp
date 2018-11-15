#include "sealib/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(GraphIOTest, exportGML) {
    Graph *g = GraphCreator::createWindmill(3, 4);
    GraphExporter::exportGML(g, "graphio1.gml");
}

TEST(GraphIOTest, importGML) {
    BasicGraph g = GraphImporter::importGML<BasicGraph>("graphio1.gml");
    EXPECT_EQ(g.getOrder(), 9);
    for (uint u = 0; u < g.getOrder()-1; u++) {
        EXPECT_EQ(g.getNodeDegree(u), 2);
    }
    EXPECT_EQ(g.getNodeDegree(g.getOrder()-1), 8);
}
