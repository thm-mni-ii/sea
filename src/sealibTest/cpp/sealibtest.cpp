#include <gtest/gtest.h>
#include "sealib.h"

using namespace math;

TEST(SealibTest, CheckCapitalisation) {
    EXPECT_EQ(SimpleMath::add(4, 6), 10);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}