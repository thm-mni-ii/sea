#include <gtest/gtest.h>
#include <sealib/dictionary/choicedictionary.h>

#include <array>
#include <random>


using Sealib::ChoiceDictionary;

TEST(ChoiceDictionaryTest, choicedictionary_integrity) {
    const uint64_t size = 25000;
    const uint64_t setSize = 5000;
    ChoiceDictionary *c = new ChoiceDictionary(size);
    std::array<uint64_t, setSize> set;

    uint64_t num = 0;
    uint32_t seed = 0;

    for (uint64_t i = 0; i < setSize; i++) {
        set[i] = num;
        num += 5;
    }

    std::shuffle(set.begin(), set.end(), std::default_random_engine(seed));

    // test zero initialization
    std::vector<uint64_t> nonZero;
    for (uint64_t i = 0; i < size; i++) {
        if (c->get(i) == 1) nonZero.push_back(i);
    }
    ASSERT_EQ(nonZero.size(), 0);

    // insert into Choice Dictionary and test if choice() returns the correct
    c->insert(123UL);
    ASSERT_EQ(c->choice(), 123);
    c->insert(94UL);
    ASSERT_EQ(c->choice(), 94);
    c->insert(63UL);
    ASSERT_EQ(c->choice(), 63);
    c->insert(37UL);
    ASSERT_EQ(c->choice(), 37);
    c->insert(23UL);
    ASSERT_EQ(c->choice(), 23);
    c->insert(12UL);
    ASSERT_EQ(c->choice(), 12);
    c->insert(12000);
    c->insert(12003);
    c->insert(12006);
    c->insert(20000);
    c->remove(12000);
    ASSERT_FALSE(c->get(12000));
    ASSERT_TRUE(c->get(12003));
    ASSERT_TRUE(c->get(12006));

    ASSERT_TRUE(c->get(20000));


    // verify inserted values
    ASSERT_TRUE(c->get(63));
    ASSERT_TRUE(c->get(37));
    ASSERT_TRUE(c->get(23));
    ASSERT_TRUE(c->get(12));
    ASSERT_TRUE(c->get(123));
    ASSERT_TRUE(c->get(94));

    // verify empty values don't return false positives
    ASSERT_FALSE(c->get(1));
    ASSERT_FALSE(c->get(7));
    ASSERT_FALSE(c->get(8));
    ASSERT_FALSE(c->get(111));

    for (uint64_t number : set) {
        c->insert(number);
        ASSERT_TRUE(c->get(number));
    }

    delete c;
}
