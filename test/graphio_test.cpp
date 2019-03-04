#include "sealib/graph/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graph/graphcreator.h"

namespace Sealib {

TEST(GraphIOTest, exportGML) {
    UndirectedGraph g = GraphCreator::sparseUndirected(1e5);
    GraphExporter::exportGML(g, false, "graphio1.gml");
}

TEST(GraphIOTest, importGML) {
    UndirectedGraph g = GraphImporter::importGML<UndirectedGraph>("graphio1.gml");
    EXPECT_EQ(g.getOrder(), 1e5);
}

}  // namespace Sealib
