#include <gtest/gtest.h>
#include <sealib/graphcreator.h>
#include <sealib/simpletrailstructure.h>
#include <sealib/eulertrail.h>
#include <stdlib.h>

using Sealib::GraphCreator;
using Sealib::BasicGraph;
using Sealib::GraphAlgorithm;
using Sealib::Graph;

TEST(GraphTest, hierholzer_simple_trail) {
    unsigned int order = 6;
    auto **adj_mtrx = (unsigned int **) malloc(sizeof(unsigned int) * order * order);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1, 1, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0, 0, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1, 0, 0};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0, 0, 0};
    adj_mtrx[4] = new unsigned int[order]{1, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new unsigned int[order]{1, 0, 0, 0, 0, 0};

    std::shared_ptr<BasicGraph>
        graph_ptr(new BasicGraph(GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order)));
    Sealib::EulerTrail<Sealib::SimpleTrailStructure> et(graph_ptr);
}
