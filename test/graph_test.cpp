//
// Created by jmeintrup on 11.03.18.
//

#include <gtest/gtest.h>
#include <malloc.h>
#include "sealib/graph.h"

TEST(GraphTest, graph_integrity) {
    unsigned int order = 4;
    unsigned int **adj_mtrx = (unsigned int**) std::malloc(sizeof(unsigned int) * 4 * 4);
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
    adj_mtrx[0] = new unsigned int[order]{0,2,0,1};
    adj_mtrx[1] = new unsigned int[order]{2,0,1,0};
    adj_mtrx[2] = new unsigned int[order]{0,1,0,1};
    adj_mtrx[3] = new unsigned int[order]{1,0,1,0};

    Graph *g = new Graph(adj_mtrx, order);

    //verify correct order and degree of nodes
    ASSERT_EQ(g->get_order(), order);
    ASSERT_EQ(g->get_node(0)->get_deg(), 3);
    ASSERT_EQ(g->get_node(1)->get_deg(), 3);
    ASSERT_EQ(g->get_node(2)->get_deg(), 2);
    ASSERT_EQ(g->get_node(3)->get_deg(), 2);

    //verify the adj vector contents of each node
    ASSERT_EQ(g->get_node(0)->get_adj()->at(0).vertex, 1);
    ASSERT_EQ(g->get_node(0)->get_adj()->at(1).vertex, 1);
    ASSERT_EQ(g->get_node(0)->get_adj()->at(2).vertex, 3);

    ASSERT_EQ(g->get_node(1)->get_adj()->at(0).vertex, 0);
    ASSERT_EQ(g->get_node(1)->get_adj()->at(1).vertex, 0);
    ASSERT_EQ(g->get_node(1)->get_adj()->at(2).vertex, 2);

    ASSERT_EQ(g->get_node(2)->get_adj()->at(0).vertex, 1);
    ASSERT_EQ(g->get_node(2)->get_adj()->at(1).vertex, 3);

    ASSERT_EQ(g->get_node(3)->get_adj()->at(0).vertex, 0);
    ASSERT_EQ(g->get_node(3)->get_adj()->at(1).vertex, 2);
}