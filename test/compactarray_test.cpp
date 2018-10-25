#include <gtest/gtest.h>
#include <sealib/compactarray.h>
#include <stdexcept>

using Sealib::CompactArray;

TEST(CompactArrayTest, insertAndGet) {
  CompactArray *a = new CompactArray(1500, 6);
  for (uint b = 0; b < 1500; b++) a->insert(b, 0);
  for (unsigned int b = 0; b < 50; b++) a->insert(b, 5);
  for (unsigned int b = 0; b < 50; b++) EXPECT_EQ(a->get(b), 5);
  for (unsigned int b = 50; b < 1500; b++) EXPECT_EQ(a->get(b), 0);
  a->insert(60, 1);
  EXPECT_EQ(a->get(60), 1);
  a->insert(1499, 1);
  EXPECT_EQ(a->get(1499), 1);
  delete a;
}
