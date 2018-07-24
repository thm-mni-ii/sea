#include "../src/segmentstack.h"
#include <gtest/gtest.h>

using Sealib::SegmentStack;

#define pushn(i, n) \
  for (int a = (i); a < (n); a++) s->push(std::make_tuple((a), 0))
#define popexp(n, exp) \
  for (int a = 0; a < (n); a++) EXPECT_EQ(s->pop(&r), (exp))

SegmentStack *s;
class SegmentStackTest : public ::testing::TestWithParam<bool> {
 protected:
  virtual void SetUp() {
    printf("-- setup --\n");
    s = new SegmentStack(20, 3, GetParam());
  }
  virtual void TearDown() {
    printf("-- teardown --\n");
    delete s;
  }
};

INSTANTIATE_TEST_CASE_P(ParamTests, SegmentStackTest,
                        ::testing::Values(true, false),
                        ::testing::PrintToStringParamName());

/*TEST_P(SegmentStackTest, usingTrailers) {
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
}*/

TEST_P(SegmentStackTest, lowAlign) {
  State r;
  pushn(0, 7);
  popexp(4, 0);
  EXPECT_EQ(s->pop(&r), DFS_DO_RESTORE);
  s->saveTrailer();
  s->dropAll();
  pushn(0, 3);
  EXPECT_TRUE(s->isAligned());
}
