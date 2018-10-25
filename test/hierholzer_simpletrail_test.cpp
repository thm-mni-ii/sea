#include <gtest/gtest.h>
#include <sealib/graphcreator.h>
#include <sealib/simpletrailstructure.h>
#include <sealib/eulertrail.h>
#include <stdlib.h>

TEST(GraphTest, hierholzer_simple_trail) {
    uint32_t order = 6;
    auto **adj_mtrx = reinterpret_cast<uint32_t **>(malloc(sizeof(uint32_t) * order * order));
    adj_mtrx[0] = new uint32_t[order]{0, 1, 0, 1, 1, 1};
    adj_mtrx[1] = new uint32_t[order]{1, 0, 1, 0, 0, 0};
    adj_mtrx[2] = new uint32_t[order]{0, 1, 0, 1, 0, 0};
    adj_mtrx[3] = new uint32_t[order]{1, 0, 1, 0, 0, 0};
    adj_mtrx[4] = new uint32_t[order]{1, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new uint32_t[order]{1, 0, 0, 0, 0, 0};

    std::shared_ptr<Sealib::BasicGraph>
        graph_ptr(
            new Sealib::BasicGraph(Sealib::GraphCreator::createGraphFromAdjacencyMatrix(
                adj_mtrx,
                order)));
    Sealib::EulerTrail<Sealib::SimpleTrailStructure> et(graph_ptr);
}
