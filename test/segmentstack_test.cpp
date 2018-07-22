#include "../src/segmentstack.h"
#include <gtest/gtest.h>

using Sealib::SegmentStack;

class SegmentStackTest : public ::testing::TestWithParam<bool> {};

INSTANTIATE_TEST_CASE_P(ParamTests, SegmentStackTest,
                        ::testing::Values(true, false),
                        ::testing::PrintToStringParamName());

TEST_P(SegmentStackTest, usingTrailers) {
  SegmentStack *s = new SegmentStack(20, 3, GetParam());
  EXPECT_TRUE(s->empty());
  EXPECT_THROW(s->saveTrailer(), std::logic_error);
  for (int a = 0; a < 7; a++) s->push(std::make_tuple(a, 0));
  State r;
  for (int a = 0; a < 4; a++) EXPECT_EQ(s->pop(&r), 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  for (int a = 0; a < 3; a++) {
    EXPECT_FALSE(s->isAligned());
    s->push(std::make_tuple(a, 0));
  }
  EXPECT_TRUE(s->isAligned());  // low segment must aligned (because there is
                                // just 1 segment left)
  for (int a = 3; a < 10; a++) s->push(std::make_tuple(a, 0));
  for (int a = 0; a < 4; a++) EXPECT_EQ(s->pop(&r), 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  for (int a = 0; a < 6; a++) {
    EXPECT_FALSE(s->isAligned());
    s->push(std::make_tuple(a, 0));
  }
  EXPECT_TRUE(s->isAligned());  // high segment must be aligned (because there
                                // are 2 segments left)
  for (int a = 0; a < 6; a++) EXPECT_EQ(s->pop(&r), 0);
  EXPECT_EQ(s->pop(&r), DFS_NO_MORE_NODES);
  EXPECT_TRUE(s->empty());
}
