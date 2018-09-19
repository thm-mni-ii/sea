#include "sealib/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(GraphIO, exportGML) {
  Graph *g = GraphCreator::createRandomGenerated(10);
  GraphExporter::exportGML(g, "graphio1.gml");
}
