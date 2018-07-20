#include <gtest/gtest.h>
#include <sealib/compactarray.h>
#include <stdexcept>

using Sealib::CompactArray;

TEST(CompactArrayTest, insertAndGet) {
  CompactArray *a = new CompactArray(1500, 0.2);
  for (int b = 0; b < 50; b++) a->insert(b, 3);
  for (int b = 0; b < 50; b++) EXPECT_EQ(a->get(b), 3);
  for (int b = 50; b < 1500; b++) EXPECT_EQ(a->get(b), 0);
  a->insert(60, 1);
  EXPECT_EQ(a->get(60), 1);
  a->insert(1499, 1);
  EXPECT_EQ(a->get(1499), 1);
  EXPECT_THROW(a->get(1500), std::out_of_range);
  EXPECT_THROW(a->insert(1500, 1), std::out_of_range);
  delete a;
}
