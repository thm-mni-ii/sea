#include "../src/segmentstack.h"
#include <gtest/gtest.h>

using Sealib::SegmentStack;

TEST(SegmentStackTest, withTrailers) {
  SegmentStack *s = new SegmentStack(20, 3, true);
  ASSERT_TRUE(s->empty());
  ASSERT_THROW(s->saveTrailer(), std::logic_error);
  for (int a = 0; a < 7; a++) s->push(std::make_tuple(a, 0));
  State r;
  for (int a = 0; a < 4; a++) ASSERT_EQ(s->pop(&r), 0);
  ASSERT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  for (int a = 0; a < 3; a++) {
    ASSERT_FALSE(s->isAligned());
    s->push(std::make_tuple(a, 0));
  }
  ASSERT_TRUE(s->isAligned());  // low segment must aligned (because there is
                                // just 1 segment left)
  for (int a = 3; a < 10; a++) s->push(std::make_tuple(a, 0));
  for (int a = 0; a < 4; a++) ASSERT_EQ(s->pop(&r), 0);
  ASSERT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  for (int a = 0; a < 6; a++) {
    ASSERT_FALSE(s->isAligned());
    s->push(std::make_tuple(a, 0));
  }
  ASSERT_TRUE(s->isAligned());  // high segment must be aligned (because there
                                // are 2 segments left)
  for (int a = 0; a < 6; a++) ASSERT_EQ(s->pop(&r), 0);
  ASSERT_EQ(s->pop(&r), DFS_NO_MORE_NODES);
  ASSERT_TRUE(s->empty());
}

TEST(SegmentStackTest, withoutTrailers) {
  SegmentStack *s = new SegmentStack(20, 3, false);
  ASSERT_TRUE(s->empty());
  ASSERT_THROW(s->saveTrailer(), std::logic_error);
  for (int a = 0; a < 7; a++) s->push(std::make_tuple(a, 0));
  State r;
  for (int a = 0; a < 4; a++) ASSERT_EQ(s->pop(&r), 0);
  ASSERT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  for (int a = 0; a < 3; a++) {
    ASSERT_FALSE(s->isAligned());
    s->push(std::make_tuple(a, 0));
  }
  ASSERT_TRUE(s->isAligned());  // low segment must aligned (because there is
                                // just 1 segment left)
  for (int a = 3; a < 10; a++) s->push(std::make_tuple(a, 0));
  for (int a = 0; a < 4; a++) ASSERT_EQ(s->pop(&r), 0);
  ASSERT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  for (int a = 0; a < 6; a++) {
    ASSERT_FALSE(s->isAligned());
    s->push(std::make_tuple(a, 0));
  }
  ASSERT_TRUE(s->isAligned());  // high segment must be aligned (because there
                                // are 2 segments left)
  for (int a = 0; a < 6; a++) ASSERT_EQ(s->pop(&r), 0);
  ASSERT_EQ(s->pop(&r), DFS_NO_MORE_NODES);
  ASSERT_TRUE(s->empty());
}
