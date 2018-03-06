#include <gtest/gtest.h>
#include "sealib/math.h"

using namespace math;

TEST(SealibTest, add) {
    EXPECT_EQ(SimpleMath::add(4, 6), 10);
}