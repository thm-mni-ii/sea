#include <gtest/gtest.h>
#include <sealib/graphcreator.h>
#include <sealib/graphalgorithm.h>
#include <sealib/simpletrailstructure.h>
#include <include/sealib/trail.h>
#include <stdlib.h>

using Sealib::GraphCreator;
using Sealib::BasicGraph;
using Sealib::GraphAlgorithm;
using Sealib::Graph;
using trail_t = Sealib::SimpleTrailStructure;

TEST(GraphTest, hierholzer_simple) {
    unsigned int order = 4;
    auto **adj_mtrx = (unsigned int **) malloc(sizeof(unsigned int) * 4 * 4);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};

    BasicGraph g = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);

    std::unique_ptr<std::vector<trail_t>> trail = GraphAlgorithm::hierholzer<trail_t>(g);
}

TEST(GraphTest, hierholzer_simple_trail) {
    unsigned int order = 4;
    auto **adj_mtrx = (unsigned int **) malloc(sizeof(unsigned int) * 4 * 4);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};

    std::shared_ptr<BasicGraph>
        graph_ptr(new BasicGraph(GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order)));
    Sealib::EulerTrail<trail_t> et(graph_ptr);

    std::cout << et << std::endl;
}
