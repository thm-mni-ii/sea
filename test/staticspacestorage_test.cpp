#include "sealib/staticspacestorage.h"
#include <gtest/gtest.h>

using Sealib::StaticSpaceStorage;

TEST(StaticSpaceStorageTest, setAndGet) {
  std::vector<bool> b = std::vector<bool>();
  b.push_back(1);
  b.push_back(0);
  b.push_back(0);
  b.push_back(0);
  b.push_back(1);
  b.push_back(0);
  StaticSpaceStorage *s = new StaticSpaceStorage(b);
  s->set(0,5);
  EXPECT_EQ(s->get(0),5);
  delete s;
}
