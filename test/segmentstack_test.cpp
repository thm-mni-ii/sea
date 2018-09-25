#include "../src/segmentstack.h"
#include <gtest/gtest.h>
#include "sealib/compactarray.h"
#include "sealib/graphcreator.h"

using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;
using Sealib::CompactArray;

#define pushn(i, n) \
  for (int a = (i); a < (n); a++) s->push(Pair((a), 0))
#define popexp(n, exp) \
  for (int a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

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
  CompactArray *c = new CompactArray(10, 5);
  Sealib::Graph *g = Sealib::GraphCreator::createRandomGenerated(5);
  virtual void SetUp() { s = new ExtendedSegmentStack(15, 3, g, c); }
  virtual void TearDown() { delete s; }
};

TEST_F(ExtendedSegmentStackTest, topSegment) {
  pushn(0, 9);
  for (uint a = 0; a < 6; a++) EXPECT_FALSE(s->isInTopSegment(a));
  for (uint a = 6; a < 9; a++) EXPECT_TRUE(s->isInTopSegment(a));
}

TEST_F(ExtendedSegmentStackTest, secondLastTrailer) {
  /* We push values 0..8 on the stack. Expected: there is only the trailer
   * (2,0), so a restoration would start from the beginning */
  pushn(0, 9);
  EXPECT_EQ(s->getRestoreTrailer(&r), 1);
  /* We push 9. Expected: Now a fourth segment is created, which results in the
   * 2 trailers (2,0) and (5,0) being stored. (2,0) will be the starting point
   * of a restoration */
  pushn(9, 10);
  EXPECT_EQ(s->getRestoreTrailer(&r), 0);
  EXPECT_EQ(r.head(), 2);
  /* We pop the two segments. Expected: A restoration is required and the
   * restore trailer will be (2,0) */
  popexp(4, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  EXPECT_EQ(s->getRestoreTrailer(&r), 0);
  EXPECT_EQ(r.head(), 2);
}

TEST_F(ExtendedSegmentStackTest, recolorLow) {
  pushn(0, 10);
  s->recolorLow(1);
  for (uint a = 0; a < 6; a++) EXPECT_EQ(c->get(a), 0);
  for (uint a = 6; a < 9; a++) EXPECT_EQ(c->get(a), 1);
  EXPECT_EQ(c->get(9), 0);
}

TEST_F(ExtendedSegmentStackTest,outgoingEdge) {
  // to do
}
