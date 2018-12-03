#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/graph/graphcreator.h>

using Sealib::GraphCreator;
using Sealib::UndirectedGraph;
using Sealib::Graph;

TEST(GraphTest, graph_integrity) {
    uint32_t order = 4;
    uint32_t **adj_mtrx = new uint32_t *[order];
    /**
     * (n)       0       1       2       3
     *      **********************************
     *  0   *    0   *   2   *   0   *   1   *
     *      **********************************
     *  1   *    2   *   0   *   1   *   0   *
     *      **********************************
     *  2   *    0   *   1   *   0   *   1   *
     *      **********************************
     *  3   *    1   *   0   *   1   *   0   *
     *      **********************************
     */
    adj_mtrx[0] = new uint32_t[order]{0, 2, 0, 1};
    adj_mtrx[1] = new uint32_t[order]{2, 0, 1, 0};
    adj_mtrx[2] = new uint32_t[order]{0, 1, 0, 1};
    adj_mtrx[3] = new uint32_t[order]{1, 0, 1, 0};

    UndirectedGraph g = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);

    // verify correct order and degree of nodes
    ASSERT_EQ(g.getOrder(), order);
    ASSERT_EQ(g.getNode(0).getDegree(), 3);
    ASSERT_EQ(g.getNode(1).getDegree(), 3);
    ASSERT_EQ(g.getNode(2).getDegree(), 2);
    ASSERT_EQ(g.getNode(3).getDegree(), 2);

    // verify the adj vector contents of each node
    ASSERT_EQ(g.getNode(0).getAdj()[0].vertex, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[1].vertex, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[2].vertex, 3);

    ASSERT_EQ(g.getNode(1).getAdj()[0].vertex, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[1].vertex, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[2].vertex, 2);

    ASSERT_EQ(g.getNode(2).getAdj()[0].vertex, 1);
    ASSERT_EQ(g.getNode(2).getAdj()[1].vertex, 3);

    ASSERT_EQ(g.getNode(3).getAdj()[0].vertex, 0);
    ASSERT_EQ(g.getNode(3).getAdj()[1].vertex, 2);

    // verify the adj vector crossindexes of each node
    ASSERT_EQ(g.getNode(0).getAdj()[0].crossIndex, 0);
    ASSERT_EQ(g.getNode(0).getAdj()[1].crossIndex, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[2].crossIndex, 0);

    ASSERT_EQ(g.getNode(1).getAdj()[0].crossIndex, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[1].crossIndex, 1);
    ASSERT_EQ(g.getNode(1).getAdj()[2].crossIndex, 0);

    ASSERT_EQ(g.getNode(2).getAdj()[0].crossIndex, 2);
    ASSERT_EQ(g.getNode(2).getAdj()[1].crossIndex, 1);

    ASSERT_EQ(g.getNode(3).getAdj()[0].crossIndex, 2);
    ASSERT_EQ(g.getNode(3).getAdj()[1].crossIndex, 1);
}
