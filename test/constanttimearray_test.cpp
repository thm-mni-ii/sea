#include <gtest/gtest.h>
#include <sealib/collection/constanttimearray.h>

using Sealib::ConstantTimeArray;

void testVariableLength(uint64_t N);
void testFill(uint64_t N, uint64_t fVal);
void setAllFromLeft(uint64_t N);
void setAllFromRight(uint64_t N);

void testVariableLength(uint64_t N) {
    ConstantTimeArray arr(N, 0);
    arr.fill(777);

    for (uint64_t i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    for (uint64_t i = 0; i < arr.size(); i++) {
        ASSERT_EQ(arr[i], i);
    }

    arr.fill(777);

    for (uint64_t i = arr.size(); i > 0; i--) {
        arr[i - 1] = i - 1;
    }

    for (uint64_t i = arr.size(); i > 0; i--) {
        ASSERT_EQ(arr[i - 1], i - 1);
    }

    arr.fill(777);

    for (uint64_t i = arr.size() / 2; i < arr.size(); i++) {
        arr[i] = i;
    }

    for (uint64_t i = arr.size() / 2; i < arr.size(); i++) {
        ASSERT_EQ(arr[i], i);
    }
}

TEST(ConstantTimeDynamicArrayTest, ArrayOperationsEvenLength) {
    const uint64_t N = 100;
    testVariableLength(N);
}

TEST(ConstantTimeArrayTest, ArrayOperationsUnevenLength) {
    const uint64_t N = 101;
    testVariableLength(N);
}
