#include <gtest/gtest.h>
#include <sealib/collection/subgraphstack.h>
#include <sealib/graph/graphcreator.h>

using Sealib::SubGraphStack;
using Sealib::SubGraph;
using Sealib::RecursiveSubGraph;
using Sealib::BaseSubGraph;
using Sealib::Bitset;
using Sealib::UndirectedGraph;
using Sealib::GraphCreator;
using std::shared_ptr;

TEST(SubGraphStackTest, pushPop) {
    typedef Sealib::Bitset<uint64_t> bitset_t;

    uint64_t order = 7;
    auto **adj_mtrx = reinterpret_cast<uint64_t **>(malloc(sizeof(uint64_t) * order * order));
    adj_mtrx[0] = new uint64_t[order]{0, 1, 0, 1, 0, 1, 1};
    adj_mtrx[1] = new uint64_t[order]{1, 0, 1, 0, 0, 1, 1};
    adj_mtrx[2] = new uint64_t[order]{0, 1, 0, 0, 0, 0, 1};
    adj_mtrx[3] = new uint64_t[order]{1, 0, 0, 0, 1, 1, 0};
    adj_mtrx[4] = new uint64_t[order]{0, 0, 0, 1, 0, 1, 0};
    adj_mtrx[5] = new uint64_t[order]{1, 1, 0, 1, 1, 0, 0};
    adj_mtrx[6] = new uint64_t[order]{1, 1, 1, 0, 0, 0, 0};

    shared_ptr<UndirectedGraph> bg =
        GraphCreator::createSharedPointerFromAdjacencyMatrix(adj_mtrx, order);

    SubGraphStack stack(bg);

    for (uint64_t i = 0; i < 4; i++) {
        bitset_t a(stack.gMax(i));
        for (uint64_t j = 0; j < a.size(); j++) {
            a[j] = 1;
        }
        for (uint64_t j = 0; j < a.size(); j++) {
            if (j % 4 == 0) {
                std::tuple<uint64_t, uint64_t> gInv = stack.gInv(j + 1);
                std::tuple<uint64_t, uint64_t> mate
                    = stack.mate(std::get<0>(gInv), std::get<1>(gInv));
                uint64_t mateArc = stack.g(std::get<0>(mate), std::get<1>(mate));
                a[j] = 0;
                a[mateArc - 1] = 0;
            }
        }
        stack.push(a);
        ASSERT_EQ(stack.size(), i+2);
    }
    for (uint64_t i = 4; i > 0; i--) {
        stack.pop();
        ASSERT_EQ(stack.size(), i);
    }
    ASSERT_EQ(stack.size(), 1);
}

TEST(SubGraphStackTest, orderDegree) {
    typedef Sealib::Bitset<uint64_t> bitset_t;

    uint64_t order = 9;
    auto **adj_mtrx = reinterpret_cast<uint64_t **>(malloc(sizeof(uint64_t) * order * order));
    adj_mtrx[0] = new uint64_t[order]{0, 0, 0, 1, 1, 1, 0, 1, 0};
    adj_mtrx[1] = new uint64_t[order]{0, 0, 0, 1, 0, 0, 0, 1, 1};
    adj_mtrx[2] = new uint64_t[order]{0, 0, 0, 0, 1, 1, 1, 1, 0};
    adj_mtrx[3] = new uint64_t[order]{1, 1, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[4] = new uint64_t[order]{1, 0, 1, 0, 0, 1, 0, 0, 0};
    adj_mtrx[5] = new uint64_t[order]{1, 0, 1, 0, 1, 0, 0, 0, 1};
    adj_mtrx[6] = new uint64_t[order]{0, 0, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[7] = new uint64_t[order]{1, 1, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[8] = new uint64_t[order]{0, 1, 0, 0, 0, 1, 0, 0, 0};

    shared_ptr<UndirectedGraph> bg =
        GraphCreator::createSharedPointerFromAdjacencyMatrix(adj_mtrx, order);

    SubGraphStack stack(bg);

    ASSERT_EQ(stack.order(), 9);
    ASSERT_EQ(stack.degree(1), 4);
    ASSERT_EQ(stack.degree(2), 3);
    ASSERT_EQ(stack.degree(3), 4);
    ASSERT_EQ(stack.degree(4), 2);
    ASSERT_EQ(stack.degree(5), 3);
    ASSERT_EQ(stack.degree(6), 4);
    ASSERT_EQ(stack.degree(7), 1);
    ASSERT_EQ(stack.degree(8), 3);
    ASSERT_EQ(stack.degree(9), 2);

    ASSERT_EQ(stack.gMax(), 26);

    for (uint64_t i = 0; i < 4; i++) {
        bitset_t a(stack.gMax(i));
        for (uint64_t j = 0; j < a.size(); j++) {
            a[j] = 1;
        }
        for (uint64_t j = 0; j < a.size(); j++) {
            if (j % 4 == 0) {
                std::tuple<uint64_t, uint64_t> gInv = stack.gInv(j + 1);
                std::tuple<uint64_t, uint64_t> mate
                    = stack.mate(std::get<0>(gInv), std::get<1>(gInv));
                uint64_t mateArc = stack.g(std::get<0>(mate), std::get<1>(mate));
                a[j] = 0;
                a[mateArc - 1] = 0;
            }
        }
        stack.push(a);
        ASSERT_EQ(stack.size(), i+2);
    }

    // G_l -> G_0
    ASSERT_EQ(stack.phi(1), 3);
    ASSERT_EQ(stack.phi(2), 5);
    ASSERT_EQ(stack.phiInv(3), 1);
    ASSERT_EQ(stack.phiInv(5), 2);
    ASSERT_EQ(stack.psi(1), 8);
    ASSERT_EQ(stack.psi(2), 15);
    ASSERT_EQ(stack.psiInv(8), 1);
    ASSERT_EQ(stack.psiInv(15), 2);

    // G_l -> G_l-1
    ASSERT_EQ(stack.phi(4, 3, 1), 2);
    ASSERT_EQ(stack.phi(4, 3, 2), 3);
    ASSERT_EQ(stack.phi(3, 4, 2), 1);
    ASSERT_EQ(stack.phi(3, 4, 3), 2);

    ASSERT_EQ(stack.psi(4, 3, 1), 2);
    ASSERT_EQ(stack.psi(4, 3, 2), 3);
    ASSERT_EQ(stack.psi(3, 4, 2), 1);
    ASSERT_EQ(stack.psi(3, 4, 3), 2);

    // mate
    std::tuple<uint64_t, uint64_t> m = stack.mate(4, 1, 1);
    ASSERT_EQ(std::get<0>(m), 2);
    ASSERT_EQ(std::get<1>(m), 1);

    m = stack.mate(4, 2, 1);
    ASSERT_EQ(std::get<0>(m), 1);
    ASSERT_EQ(std::get<1>(m), 1);

    // head
    ASSERT_EQ(stack.head(4, 1, 1), 2);
    ASSERT_EQ(stack.head(4, 2, 1), 1);

    for (uint64_t i = 4; i > 0; i--) {
        stack.pop();
        ASSERT_EQ(stack.size(), i);
    }
    ASSERT_EQ(stack.size(), 1);
}

/**
for (uint64_t i = 0; i < 5; i++) {
        bitset_t a(stack.order(i));
        for (uint64_t j = 0; j < a.size(); j++) {
            a[j] = j % 4 != 0;
        }
        stack.push_vertex_induced(a);
    }*/

TEST(SubGraphStackTest, vertexInduced) {
    typedef Sealib::Bitset<uint64_t> bitset_t;

    uint64_t order = 9;
    auto **adj_mtrx = reinterpret_cast<uint64_t **>(malloc(sizeof(uint64_t) * order * order));
    adj_mtrx[0] = new uint64_t[order]{0, 0, 0, 1, 1, 1, 0, 1, 0};
    adj_mtrx[1] = new uint64_t[order]{0, 0, 0, 1, 0, 0, 0, 1, 1};
    adj_mtrx[2] = new uint64_t[order]{0, 0, 0, 0, 1, 1, 1, 1, 0};
    adj_mtrx[3] = new uint64_t[order]{1, 1, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[4] = new uint64_t[order]{1, 0, 1, 0, 0, 1, 0, 0, 0};
    adj_mtrx[5] = new uint64_t[order]{1, 0, 1, 0, 1, 0, 0, 0, 1};
    adj_mtrx[6] = new uint64_t[order]{0, 0, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[7] = new uint64_t[order]{1, 1, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[8] = new uint64_t[order]{0, 1, 0, 0, 0, 1, 0, 0, 0};

    shared_ptr<UndirectedGraph> bg =
        GraphCreator::createSharedPointerFromAdjacencyMatrix(adj_mtrx, order);

    SubGraphStack stack(bg);

    ASSERT_EQ(stack.order(), 9);
    ASSERT_EQ(stack.degree(1), 4);
    ASSERT_EQ(stack.degree(2), 3);
    ASSERT_EQ(stack.degree(3), 4);
    ASSERT_EQ(stack.degree(4), 2);
    ASSERT_EQ(stack.degree(5), 3);
    ASSERT_EQ(stack.degree(6), 4);
    ASSERT_EQ(stack.degree(7), 1);
    ASSERT_EQ(stack.degree(8), 3);
    ASSERT_EQ(stack.degree(9), 2);

    ASSERT_EQ(stack.gMax(), 26);

    for (uint64_t i = 0; i < 3; i++) {
        bitset_t a(stack.order());
        size_t new_order = 0;
        for (uint64_t j = 0; j < a.size(); j++) {
            if (j % 4 != 0) {
                a[j] = true;
                new_order++;
            } else {
                a[j] = false;
            }
        }
        stack.push_vertex_induced(a);
        ASSERT_EQ(stack.size(), i+2);
        ASSERT_EQ(stack.order(), new_order);
    }
}
