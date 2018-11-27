#include <gtest/gtest.h>
#include <sealib/trackingallocator.h>
#include <sealib/bytecounter.h>
#include <vector>

TEST(TrackingAllocatorTest, vectorTest) {
    ASSERT_EQ(Sealib::ByteCounter::get(), 0);
    std::vector<int, Sealib::TrackingAllocator<int>> a(5);
    ASSERT_EQ(Sealib::ByteCounter::get(), 20);
    std::vector<uint32_t, Sealib::TrackingAllocator<uint32_t>> b(5);
    ASSERT_EQ(Sealib::ByteCounter::get(), 40);
    std::vector<uint8_t, Sealib::TrackingAllocator<uint8_t>> c(5);
    ASSERT_EQ(Sealib::ByteCounter::get(), 45);
}
