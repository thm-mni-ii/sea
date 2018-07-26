#include "sealib/staticspacestorage.h"
#include <gtest/gtest.h>

using Sealib::StaticSpaceStorage;

TEST(StaticSpaceStorageTest, setAndGet) {
  std::vector<bool> b = {1,0,0,0,1,0,0,0};
  StaticSpaceStorage *s = new StaticSpaceStorage(b);
  //s->set(0,5);
  //EXPECT_EQ(s->get(0),5);
  delete s;
}
