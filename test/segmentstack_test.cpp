#include "../src/segmentstack.h"
#include <gtest/gtest.h>

using Sealib::BasicSegmentStack;

#define pushn(i, n) \
  for (int a = (i); a < (n); a++) s->push(Pair((a), 0))
#define popexp(n, exp) \
  for (int a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

static BasicSegmentStack *s = new BasicSegmentStack(3);
static Pair r;

TEST(BasicSegmentStackTest, empty) {
  EXPECT_TRUE(s->empty());
  EXPECT_THROW(s->saveTrailer(), std::logic_error);
  pushn(0, 1);
  EXPECT_FALSE(s->empty());
  popexp(1, 0);
  EXPECT_TRUE(s->empty());
  popexp(1, DFS_NO_MORE_NODES);
}
TEST(BasicSegmentStackTest, lowAlign) {
  pushn(0, 7);
  popexp(4, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  pushn(0, 3);
  EXPECT_TRUE(s->isAligned());
}
TEST(BasicSegmentStackTest, highAlign) {
  pushn(0, 10);
  popexp(4, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  pushn(0, 6);
  EXPECT_TRUE(s->isAligned());
}
