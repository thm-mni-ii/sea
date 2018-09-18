#include "../src/segmentstack.h"
#include <gtest/gtest.h>

using Sealib::SegmentStack;

#define pushn(i, n) \
  for (int a = (i); a < (n); a++) s->push(Pair((a), 0))
#define popexp(n, exp) \
  for (int a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

SegmentStack *s;
Pair r;
class SegmentStackTest : public ::testing::TestWithParam<bool> {
 protected:
  virtual void SetUp() { s = new SegmentStack(20, 3, GetParam()); }
  virtual void TearDown() { delete s; }
};

INSTANTIATE_TEST_CASE_P(ParamTests, SegmentStackTest,
                        ::testing::Values(false, true),
                        /**/);

TEST_P(SegmentStackTest, empty) {
  EXPECT_TRUE(s->empty());
  EXPECT_THROW(s->saveTrailer(), std::logic_error);
  pushn(0, 1);
  EXPECT_FALSE(s->empty());
  popexp(1, 0);
  EXPECT_TRUE(s->empty());
  popexp(1, DFS_NO_MORE_NODES);
}
TEST_P(SegmentStackTest, lowAlign) {
  pushn(0, 7);
  popexp(4, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  pushn(0, 3);
  EXPECT_TRUE(s->isAligned());
}
TEST_P(SegmentStackTest, highAlign) {
  pushn(0, 10);
  popexp(4, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  pushn(0, 6);
  EXPECT_TRUE(s->isAligned());
}
