//
// Created by jmeintrup on 11.03.18.
//

#include <gtest/gtest.h>
#include <malloc.h>
#include "sealib/graph.h"

TEST(GraphTest, graph_integrity) {
    int order = 4;
    int **adj_mtrx = (int**) std::malloc(sizeof(int) * 4 * 4);
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
    adj_mtrx[0] = new int[order]{0,2,0,1};
    adj_mtrx[1] = new int[order]{2,0,1,0};
    adj_mtrx[2] = new int[order]{0,1,0,1};
    adj_mtrx[3] = new int[order]{1,0,1,0};

    Graph *g = new Graph(adj_mtrx, order);

    //verify correct order and degree of nodes
    ASSERT_EQ(g->get_order(), order);
    ASSERT_EQ(g->get_node(0)->get_deg(), 3);
    ASSERT_EQ(g->get_node(1)->get_deg(), 3);
    ASSERT_EQ(g->get_node(2)->get_deg(), 2);
    ASSERT_EQ(g->get_node(3)->get_deg(), 2);

    //verify the adj vector contents of each node
    ASSERT_EQ(g->get_node(0)->getAdj()->at(0).get_value(), 1);
    ASSERT_EQ(g->get_node(0)->getAdj()->at(1).get_value(), 1);
    ASSERT_EQ(g->get_node(0)->getAdj()->at(2).get_value(), 3);

    ASSERT_EQ(g->get_node(1)->getAdj()->at(0).get_value(), 0);
    ASSERT_EQ(g->get_node(1)->getAdj()->at(1).get_value(), 0);
    ASSERT_EQ(g->get_node(1)->getAdj()->at(2).get_value(), 2);

    ASSERT_EQ(g->get_node(2)->getAdj()->at(0).get_value(), 1);
    ASSERT_EQ(g->get_node(2)->getAdj()->at(1).get_value(), 3);

    ASSERT_EQ(g->get_node(3)->getAdj()->at(0).get_value(), 0);
    ASSERT_EQ(g->get_node(3)->getAdj()->at(1).get_value(), 2);

    //verify links of node 0
    Node* n0 = g->get_node(0);
    ASSERT_EQ(n0->getAdj()->at(0).get_match(), &g->get_node(1)->getAdj()->at(0));
    ASSERT_EQ(n0->getAdj()->at(1).get_match(), &g->get_node(1)->getAdj()->at(1));
    ASSERT_EQ(n0->getAdj()->at(1).get_match(), &g->get_node(1)->getAdj()->at(1));

    ASSERT_EQ(&n0->getAdj()->at(0), g->get_node(1)->getAdj()->at(0).get_match());
    ASSERT_EQ(&n0->getAdj()->at(1), g->get_node(1)->getAdj()->at(1).get_match());
    ASSERT_EQ(&n0->getAdj()->at(1), g->get_node(1)->getAdj()->at(1).get_match());
    /*ASSERT_EQ(g->get_node(0)->getAdj()->at(1).get_value(), 1);
    ASSERT_EQ(g->get_node(0)->getAdj()->at(2).get_value(), 3);*/
}