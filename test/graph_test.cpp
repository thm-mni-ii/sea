#include <gtest/gtest.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/graph/virtualgraph.h>
#include <stdlib.h>

using namespace Sealib;  // NOLINT

TEST(GraphTest, graph_integrity) {
    uint64_t order = 4;
    uint64_t **adj_mtrx = new uint64_t *[order];
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
    adj_mtrx[0] = new uint64_t[order]{0, 2, 0, 1};
    adj_mtrx[1] = new uint64_t[order]{2, 0, 1, 0};
    adj_mtrx[2] = new uint64_t[order]{0, 1, 0, 1};
    adj_mtrx[3] = new uint64_t[order]{1, 0, 1, 0};

    UndirectedGraph g =
        GraphCreator::createFromAdjacencyMatrix(adj_mtrx, order);

    // verify correct order and degree of nodes
    ASSERT_EQ(g.getOrder(), order);
    ASSERT_EQ(g.getNode(0).getDegree(), 3);
    ASSERT_EQ(g.getNode(1).getDegree(), 3);
    ASSERT_EQ(g.getNode(2).getDegree(), 2);
    ASSERT_EQ(g.getNode(3).getDegree(), 2);

    // verify the adj vector contents of each node
    ASSERT_EQ(g.getNode(0).getAdj()[0].first, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[1].first, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[2].first, 3);

    ASSERT_EQ(g.getNode(1).getAdj()[0].first, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[1].first, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[2].first, 2);

    ASSERT_EQ(g.getNode(2).getAdj()[0].first, 1);
    ASSERT_EQ(g.getNode(2).getAdj()[1].first, 3);

    ASSERT_EQ(g.getNode(3).getAdj()[0].first, 0);
    ASSERT_EQ(g.getNode(3).getAdj()[1].first, 2);

    // verify the adj vector crossindexes of each node
    ASSERT_EQ(g.getNode(0).getAdj()[0].second, 0);
    ASSERT_EQ(g.getNode(0).getAdj()[1].second, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[2].second, 0);

    ASSERT_EQ(g.getNode(1).getAdj()[0].second, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[1].second, 1);
    ASSERT_EQ(g.getNode(1).getAdj()[2].second, 0);

    ASSERT_EQ(g.getNode(2).getAdj()[0].second, 2);
    ASSERT_EQ(g.getNode(2).getAdj()[1].second, 1);

    ASSERT_EQ(g.getNode(3).getAdj()[0].second, 2);
    ASSERT_EQ(g.getNode(3).getAdj()[1].second, 1);
}

TEST(VirtualGraphTest, basicOperations) {
    uint32_t n = 100, deg = 15;
    UndirectedGraph baseGraph = GraphCreator::kRegular(n, deg);
    VirtualGraph g(baseGraph);
    EXPECT_EQ(g.getOrder(), n);
    for (uint32_t a = 0; a < n; a++) {
        EXPECT_EQ(g.deg(a), deg);
        for (uint b = 0; b < deg; b++) {
            EXPECT_EQ(g.head(a, b), baseGraph.head(a, b));
        }
    }

    g.addEdge(3, 33);
    ASSERT_EQ(g.deg(3), deg + 1);
    EXPECT_EQ(g.head(3, deg), 33);
    EXPECT_EQ(g.head(33, deg), 3);
    g.removeEdge(3, 33);
    ASSERT_EQ(g.deg(3), deg);
    uint64_t removed = g.head(3, 7);
    std::vector<uint64_t> adj;
    for (uint64_t k = 0; k < g.deg(3); k++) {
        if (g.head(3, k) != removed) {
            adj.emplace_back(g.head(3, k));
        }
    }
    g.removeVertex(removed);

    for (uint64_t a = 0; a < adj.size(); a++) {
        EXPECT_EQ(g.head(3, a), adj[a]) << "failed at " << a;
    }
    for (uint64_t a = adj.size(); a < deg; a++) {
        EXPECT_EQ(g.head(3, a), INVALID);
    }
}

TEST(VirtualGraphTest, mate) {
    uint64_t n = 100, deg = 8;
    UndirectedGraph baseGraph = GraphCreator::kRegular(n, deg);
    VirtualGraph g(baseGraph);
    for (uint64_t u = 0; u < n; u++) {
        for (uint64_t k = 0; k < g.deg(u); k++) {
            ASSERT_EQ(g.mate(u, k), baseGraph.mate(u, k));
        }
    }

    g.addEdge(20, 30);
    g.addEdge(40, 20);
    EXPECT_EQ(g.mate(20, deg), deg);
    EXPECT_EQ(g.mate(30, deg), deg);
    EXPECT_EQ(g.mate(20, deg + 1), deg);
    EXPECT_EQ(g.mate(40, deg), deg + 1);

    g.removeEdge(30, 20);
    EXPECT_EQ(g.mate(20, deg), deg);
    EXPECT_EQ(g.mate(40, deg), deg);

    g.removeEdge(20, 40);
    EXPECT_EQ(g.mate(20, deg), INVALID);
    EXPECT_EQ(g.mate(30, deg), INVALID);
    EXPECT_EQ(g.mate(40, deg), INVALID);
}

TEST(VirtualGraphTest, virtualEdges) {
    UndirectedGraph baseGraph = GraphCreator::windmill(5, 8);
    VirtualGraph g(baseGraph);
    g.addEdge(0, 12);
    g.addEdge(20, 0);
    ASSERT_EQ(g.deg(0), baseGraph.deg(0) + 2);
    ASSERT_EQ(g.deg(12), baseGraph.deg(12) + 1);
    ASSERT_EQ(g.deg(20), baseGraph.deg(20) + 1);
    EXPECT_EQ(g.head(0, baseGraph.deg(0)), 12);
    EXPECT_EQ(g.head(0, baseGraph.deg(0) + 1), 20);
    EXPECT_EQ(g.head(12, baseGraph.deg(12)), 0);
    EXPECT_EQ(g.head(20, baseGraph.deg(20)), 0);

    g.removeEdge(12, 0);
    ASSERT_EQ(g.deg(0), baseGraph.deg(0) + 1);
    ASSERT_EQ(g.deg(12), baseGraph.deg(12));
    ASSERT_EQ(g.deg(20), baseGraph.deg(20) + 1);
    EXPECT_EQ(g.head(0, baseGraph.deg(0)), 20);
    EXPECT_EQ(g.head(20, baseGraph.deg(20)), 0);

    g.removeEdge(0, 20);
    ASSERT_EQ(g.deg(0), baseGraph.deg(0));
    ASSERT_EQ(g.deg(20), baseGraph.deg(20));

    for (uint64_t u = 0; u < g.getOrder() / log2(g.getOrder()); u++) {
        g.addEdge(u, u + 1);
    }
}
