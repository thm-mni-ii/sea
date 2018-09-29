#include "../src/segmentstack.h"
#include <gtest/gtest.h>
#include "sealib/compactarray.h"
#include "sealib/graphcreator.h"

using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;
using Sealib::CompactArray;

#define pushn(i, n) \
  for (unsigned a = (i); a < (n); a++) s->push(Pair((a), K))
#define popexp(n, exp) \
  for (unsigned a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

static Pair r;
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
  ExtendedSegmentStack *s, *s2;
  unsigned q, q2;
  CompactArray *c, *c2;
  Sealib::Graph *g, *g2;
  virtual void SetUp() {
    unsigned n = 256;
    g = Sealib::GraphCreator::createRandomFixed(n, 10);
    c = new CompactArray(n, 3);
    s = new ExtendedSegmentStack(n, g, c);
    q = static_cast<unsigned>(ceil(n / log2(n)));
    unsigned n2 = 16;
    g2 = Sealib::GraphCreator::createRandomImbalanced(n2);
    c2 = new CompactArray(n2, 3);
    s2 = new ExtendedSegmentStack(n2, g2, c2);
    q2 = static_cast<unsigned>(ceil(n2 / log2(n2)));
  }
  virtual void TearDown() { delete s; }
};

TEST_F(ExtendedSegmentStackTest, topSegment) {
  /* We fill 3 segments. Expected: The top segment will be correctly recognized
   */
  pushn(0, 3 * q);
  for (uint a = 0; a < 2 * q; a++) EXPECT_FALSE(s->isInTopSegment(a));
  for (uint a = 2 * q; a < 3 * q; a++) EXPECT_TRUE(s->isInTopSegment(a));
  /* We push one value to the fourth segment. Expected: Only the new value is
   * now in a top segment */
  s->push(Pair(3 * q, K));
  for (uint a = 0; a < 3 * q; a++) EXPECT_FALSE(s->isInTopSegment(a));
  EXPECT_TRUE(s->isInTopSegment(3 * q));
  /* We pop the top value, which will leave us with 3 segments. Expected: Again,
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
  EXPECT_EQ(r.head(), q - 1);
  /* We pop the two segments. Expected: A restoration is required and the
   * restore trailer is (A,K) as predicted */
  popexp(q + 1, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  EXPECT_EQ(s->getRestoreTrailer(&r), 0);
  EXPECT_EQ(r.head(), q - 1);
}

TEST_F(ExtendedSegmentStackTest, recolorLow) {
  pushn(0, 3 * q + 1);
  s->recolorLow(1);
  for (uint a = 0; a < 2 * q; a++) EXPECT_EQ(c->get(a), 0);
  for (uint a = 2 * q; a < 3 * q; a++) EXPECT_EQ(c->get(a), 1);
  EXPECT_EQ(c->get(3 * q), 0);
}

TEST_F(ExtendedSegmentStackTest, outgoingEdgeSmall) {
  /* We push a small vertex and check the outgoing edge. Expected: the returned
   * outgoing edge will be the first in the unexplored group */
  // m/q=2560/(256/8)=2560/32=80 => all vertices in G are small (10 < 80)
  // gu=ceil(deg(u)/ld(n))=ceil(10/8)=2
  // fu=floor((k-1)/gu)=floor((k-1)/2)

  // storing: k=1 => fu=floor((k-1)/2)=floor(0/2)=0
  // retrieving: fu=0 => k'=fu*gu=0*2=0
  s->push(Pair(0, 1));
  EXPECT_EQ(s->getOutgoingEdge(0), 0);
  s->push(Pair(0, 2));
  EXPECT_EQ(s->getOutgoingEdge(0), 0);
  s->push(Pair(0, 3));
  EXPECT_EQ(s->getOutgoingEdge(0), 2);
  s->push(Pair(0, 11));
  EXPECT_EQ(s->getOutgoingEdge(0), 10);
}

TEST_F(ExtendedSegmentStackTest, outgoingEdgeBig) {
  for (uint a = 0; a < g2->getOrder(); a++)
    s2->push(Pair(a, g2->getNodeDegree(a) - 1));
  // ...
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
    s->push(Pair(a, K));
  }
  EXPECT_TRUE(s->isAligned());
  /* We pop the segment again and restore. Expected: The restoration is
   * successful */
  popexp(q, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  for (uint a = 0; a < q; a++) {
    EXPECT_FALSE(s->isAligned());
    s->push(Pair(a, K));
  }
  EXPECT_TRUE(s->isAligned());
  /* We replace the top element with a wrong one. Expected: the trailer does not
   * match */
  s->pop(&r);
  EXPECT_FALSE(s->isAligned());
  s->push(Pair(0, K));
  EXPECT_FALSE(s->isAligned());
  popexp(q, 0);
  EXPECT_EQ(s->pop(&r), DFS_NO_MORE_NODES);
}
