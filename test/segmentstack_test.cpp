#include "../src/segmentstack.h"
#include <gtest/gtest.h>
#include "sealib/compactarray.h"
#include "sealib/graphcreator.h"

using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;
using Sealib::CompactArray;

#define pushn(i, n) \
  for (unsigned a = (i); a < (n); a++) s->push(Pair((a), 0))
#define popexp(n, exp) \
  for (unsigned a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

static Pair r;

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
  unsigned q;
  CompactArray *c;
  Sealib::Graph *g = Sealib::GraphCreator::createRandomGenerated(100);
  virtual void SetUp() {
    unsigned n = 256;
    c = new CompactArray(n, 3);
    s = new ExtendedSegmentStack(n, g, c);
    q = static_cast<unsigned>(ceil(n / log2(n)));
  }
  virtual void TearDown() { delete s; }
};

TEST_F(ExtendedSegmentStackTest, topSegment) {
  pushn(0, 3 * q);
  for (uint a = 0; a < 2 * q; a++) EXPECT_FALSE(s->isInTopSegment(a));
  for (uint a = 2 * q; a < 3 * q; a++) EXPECT_TRUE(s->isInTopSegment(a));
}

TEST_F(ExtendedSegmentStackTest, secondLastTrailer) {
  /* We fill three segments of the stack. Expected: there is only the trailer,
   * so a restoration would start from the beginning */
  pushn(0, 3 * q);
  EXPECT_EQ(s->getRestoreTrailer(&r), 1);
  /* We push one more value. Expected: Now a fourth segment is created, which
   * results in the
   * 2 trailers A and B being stored. A will be the starting point
   * of a restoration */
  pushn(3 * q, 3 * q + 1);
  EXPECT_EQ(s->getRestoreTrailer(&r), 0);
  EXPECT_EQ(r.head(), q - 1);
  /* We pop the two segments. Expected: A restoration is required and the
   * restore trailer will be A */
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

TEST_F(ExtendedSegmentStackTest, outgoingEdge) {
  // to do
}

TEST_F(ExtendedSegmentStackTest, aligned) {
  pushn(0, 4 * q);
  popexp(2 * q, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  for (uint a = q; a < 2 * q; a++) {
    EXPECT_FALSE(s->isAligned());
    s->push(Pair(a, 0));
  }
  EXPECT_TRUE(s->isAligned());
  popexp(q, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  for (uint a = 0; a < q; a++) {
    EXPECT_FALSE(s->isAligned());
    s->push(Pair(a, 0));
  }
  EXPECT_TRUE(s->isAligned());
}
