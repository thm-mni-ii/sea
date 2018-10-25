#include <gtest/gtest.h>
#include <sealib/trackingallocator.h>
#include <sealib/bytecounter.h>
#include <vector>

TEST(TrackingAllocatorTest, vectorTest) {
    ASSERT_EQ(Sealib::ByteCounter::get(), 0);
    std::vector<int, Sealib::TrackingAllocator<int>> a(5);
    ASSERT_EQ(Sealib::ByteCounter::get(), 20);
    std::vector<unsigned int, Sealib::TrackingAllocator<unsigned int>> b(5);
    ASSERT_EQ(Sealib::ByteCounter::get(), 40);
    std::vector<unsigned char, Sealib::TrackingAllocator<unsigned char>> c(5);
    ASSERT_EQ(Sealib::ByteCounter::get(), 45);
}
