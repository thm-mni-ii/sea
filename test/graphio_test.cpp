#include "sealib/graphio.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(GraphIO, exportGML) {
  unsigned int order = 5;
  unsigned int **adj_mtrx = new unsigned int *[order];
  adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1, 1};
  adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0, 0};
  adj_mtrx[2] = new unsigned int[order]{0, 0, 0, 1, 0};
  adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 1, 1};
  adj_mtrx[4] = new unsigned int[order]{1, 0, 0, 1, 0};

  Basicgraph *g = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
  GraphExporter::exportGML(g, "graphio1.gml");
}
