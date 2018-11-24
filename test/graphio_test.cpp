#include "sealib/graph/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graph/graphcreator.h"

namespace Sealib {

TEST(GraphIOTest, exportGML) {
    UndirectedGraph g = GraphCreator::createWindmill(3, 4);
    GraphExporter::exportGML(&g, false, "graphio1.gml");
}

TEST(GraphIOTest, importGML) {
    UndirectedGraph g = GraphImporter::importGML<UndirectedGraph>("graphio1.gml");
    EXPECT_EQ(g.getOrder(), 9);
    for (uint u = 0; u < g.getOrder()-1; u++) {
        EXPECT_EQ(g.getNodeDegree(u)/2, 2);
    }
    EXPECT_EQ(g.getNodeDegree(g.getOrder()-1)/2, 8);
}

}  // namespace Sealib
