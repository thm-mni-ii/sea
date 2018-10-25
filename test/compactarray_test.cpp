#include <gtest/gtest.h>
#include <sealib/compactarray.h>
#include <stdexcept>

using Sealib::CompactArray;

TEST(CompactArrayTest, insertAndGet) {
  CompactArray *a = new CompactArray(1500, 6);
  for (uint32_t b = 0; b < 50; b++) a->insert(b, 5);
  for (uint32_t b = 0; b < 50; b++) EXPECT_EQ(a->get(b), 5);
  for (uint32_t b = 50; b < 1500; b++) EXPECT_EQ(a->get(b), 0);
  a->insert(60, 1);
  EXPECT_EQ(a->get(60), 1);
  a->insert(1499, 1);
  EXPECT_EQ(a->get(1499), 1);
  EXPECT_THROW(a->get(1500), std::out_of_range);
  EXPECT_THROW(a->insert(1500, 1), std::out_of_range);
  delete a;
}

TEST(CompactArrayTest, tooWide) {
  CompactArray a(20, 3);
  EXPECT_THROW(a.insert(0, 42), std::invalid_argument);
}
