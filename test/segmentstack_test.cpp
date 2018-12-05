#include "sealib/segmentstack.h"
#include <gtest/gtest.h>
#include "sealib/collection/compactarray.h"
#include "sealib/graph/graphcreator.h"

using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;
using Sealib::CompactArray;
using Sealib::DirectedGraph;

#define pushn(i, n) \
    for (unsigned a = (i); a < (n); a++) s->push(std::pair<uint, uint>((a), K))
#define popexp(n, exp) \
    for (uint32_t a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

static std::pair<uint, uint> r;
static const uint K = 5;

class BasicSegmentStackTest : public ::testing::Test {
 protected:
    BasicSegmentStack *s = new BasicSegmentStack(3);
    virtual void SetUp() { s->dropAll(); }
};

TEST_F(BasicSegmentStackTest, isEmpty) {
    EXPECT_TRUE(s->isEmpty());
    EXPECT_THROW(s->saveTrailer(), std::logic_error);
    pushn(0, 1);
    EXPECT_FALSE(s->isEmpty());
    popexp(1, 0);
    EXPECT_TRUE(s->isEmpty());
    popexp(1, DFS_NO_MORE_NODES);
}
TEST_F(BasicSegmentStackTest, lowAlign) {
    pushn(0, 7);
    popexp(4, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    s->saveTrailer();
    s->dropAll();
    pushn(0, 3);
    EXPECT_TRUE(s->isAligned());
}
TEST_F(BasicSegmentStackTest, highAlign) {
    pushn(0, 10);
    popexp(4, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    s->saveTrailer();
    s->dropAll();
    pushn(0, 6);
    EXPECT_TRUE(s->isAligned());
}

class ExtendedSegmentStackTest : public ::testing::Test {
 protected:
    ExtendedSegmentStack *s;
    uint32_t q;
    CompactArray *c;
    DirectedGraph g;
    virtual void SetUp() {
        uint32_t n = 256;
        g = Sealib::GraphCreator::createRandomKRegularGraph(n, 10);
        c = new CompactArray(n, 3);
        for (uint a = 0; a < n; a++) c->insert(a, 0);
        s = new ExtendedSegmentStack(n, &g, c);
        q = static_cast<uint32_t>(ceil(n / log2(n)));
    }
    virtual void TearDown() { delete s; }
};

class ExtendedSegmentStackTest2 : public ::testing::Test {
 protected:
    ExtendedSegmentStack *s;
    uint32_t q;
    CompactArray *c;
    DirectedGraph g;
    virtual void SetUp() {
        uint32_t n = 128;
        g = Sealib::GraphCreator::createRandomImbalanced(n);
        c = new CompactArray(n, 3);
        s = new ExtendedSegmentStack(n, &g, c);
        q = static_cast<uint32_t>(ceil(n / log2(n)));
    }
    virtual void TearDown() { delete s; }
};

TEST_F(ExtendedSegmentStackTest, topSegment) {
    /* We fill 3 segments. Expected: The top segment will be correctly
     * recognized
     */
    pushn(0, 3 * q);
    for (uint a = 0; a < 2 * q; a++) EXPECT_FALSE(s->isInTopSegment(a));
    for (uint a = 2 * q; a < 3 * q; a++) EXPECT_TRUE(s->isInTopSegment(a));
    /* We push one value to the fourth segment. Expected: Only the new value is
     * now in a top segment */
    s->push(std::pair<uint, uint>(3 * q, K));
    for (uint a = 0; a < 3 * q; a++) EXPECT_FALSE(s->isInTopSegment(a));
    EXPECT_TRUE(s->isInTopSegment(3 * q));
    /* We pop the top value, which will leave us with 3 segments. Expected:
     * Again,
     * the third segment is the top segment of S */
    s->pop(&r);
    for (uint a = 0; a < 2 * q; a++) EXPECT_FALSE(s->isInTopSegment(a));
    for (uint a = 2 * q; a < 3 * q; a++) EXPECT_TRUE(s->isInTopSegment(a));
    popexp(q, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    EXPECT_TRUE(s->isInTopSegment(2 * q - 1));
}

TEST_F(ExtendedSegmentStackTest, secondLastTrailer) {
    /* We fill three segments of the stack. Expected: there is only the trailer,
     * so a restoration would start from the beginning */
    pushn(0, 3 * q);
    EXPECT_EQ(s->getRestoreTrailer(&r), 1);
    /* We push one more value. Expected: Now a fourth segment is created, which
     * results in the
     * 2 trailers (A,K) and (B,K) being stored. (A,K) will be the starting point
     * of a restoration */
    pushn(3 * q, 3 * q + 1);
    EXPECT_EQ(s->getRestoreTrailer(&r), 0);
    EXPECT_EQ(r.first, q - 1);
    /* We pop the two segments. Expected: A restoration is required and the
     * restore trailer is (A,K) as predicted */
    popexp(q + 1, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    EXPECT_EQ(s->getRestoreTrailer(&r), 0);
    EXPECT_EQ(r.first, q - 1);
}

TEST_F(ExtendedSegmentStackTest, recolorLow) {
    pushn(0, 3 * q + 1);
    s->recolorLow(1);
    for (uint a = 0; a < 2 * q; a++) EXPECT_EQ(c->get(a), 0);
    for (uint a = 2 * q; a < 3 * q; a++) EXPECT_EQ(c->get(a), 1);
    EXPECT_EQ(c->get(3 * q), 0);
}

TEST_F(ExtendedSegmentStackTest, outgoingEdgeSmall) {
    /* We push a small vertex and check the outgoing edge. Expected: the
     * returned
     * outgoing edge will be the first in the unexplored group */
    // m/q=2560/(256/8)=2560/32=80 => all vertices in G are small (10 < 80)
    // gu=ceil(deg(u)/ld(n))=ceil(10/8)=2
    // fu=floor((k-1)/gu)=floor((k-1)/2)

    // storing: k=1 => fu=floor((k-1)/2)=floor(0/2)=0
    // retrieving: fu=0 => k'=fu*gu=0*2=0
    EXPECT_EQ(s->approximateEdge(0, 1), 0);
    EXPECT_EQ(s->retrieveEdge(0, 0), 0);

    EXPECT_EQ(s->approximateEdge(0, 2), 0);
    EXPECT_EQ(s->retrieveEdge(0, 0), 0);

    EXPECT_EQ(s->approximateEdge(0, 3), 1);
    EXPECT_EQ(s->retrieveEdge(0, 1), 2);

    EXPECT_EQ(s->approximateEdge(0, 6), 2);
    EXPECT_EQ(s->retrieveEdge(0, 2), 4);

    EXPECT_EQ(s->approximateEdge(0, 11), 5);
    EXPECT_EQ(s->retrieveEdge(0, 5), 10);
}

TEST_F(ExtendedSegmentStackTest2, outgoingEdgeBig) {
    uint m = 0;
    std::set<uint> big;
    for (uint u = 0; u < g.getOrder(); u++) m += g.getNodeDegree(u);
    for (uint u = 0; u < g.getOrder(); u++) {
        if (g.getNodeDegree(u) > m / q) {
            big.insert(u);
        }
    }
    for (uint a = 0; a < 4 * q; a++) {
        s->push(std::pair<uint, uint>(a, 0));
        s->pop(&r);
        s->push(std::pair<uint, uint>(a, 3));
    }
    popexp(2 * q, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    EXPECT_EQ(s->getRestoreTrailer(&r), 0);
    EXPECT_EQ(r.first, q - 1);
    EXPECT_EQ(r.second, 3);
    for (uint a = q; a < 2 * q; a++) {
        EXPECT_FALSE(s->isAligned());
        if (big.find(a) == big.end()) {
            s->push(std::pair<uint, uint>(a, 3));
        } else {
            uint b = s->getOutgoingEdge(a);
            EXPECT_EQ(b, 2);
            s->push(std::pair<uint, uint>(a, 3));
        }
    }
    EXPECT_TRUE(s->isAligned());
}

TEST_F(ExtendedSegmentStackTest, aligned) {
    /* We push 4 and completely pop 2 segments. Expected: a restoration is
     * required */
    pushn(0, 3 * q + 1);
    popexp(q + 1, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    /* We restore one segment correctly. Expected: the stack is aligned */
    for (uint a = q; a < 2 * q; a++) {
        EXPECT_FALSE(s->isAligned());
        s->push(std::pair<uint, uint>(a, K));
    }
    EXPECT_TRUE(s->isAligned());
    /* We pop the segment again and restore. Expected: The restoration is
     * successful */
    popexp(q, 0);
    EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
    for (uint a = 0; a < q; a++) {
        EXPECT_FALSE(s->isAligned());
        s->push(std::pair<uint, uint>(a, K));
    }
    EXPECT_TRUE(s->isAligned());
    /* We replace the top element with a wrong one. Expected: the trailer does
     * not
     * match */
    s->pop(&r);
    EXPECT_FALSE(s->isAligned());
    s->push(std::pair<uint, uint>(0, K));
    EXPECT_FALSE(s->isAligned());
    popexp(q, 0);
    EXPECT_EQ(s->pop(&r), DFS_NO_MORE_NODES);
}
