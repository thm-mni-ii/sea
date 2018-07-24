#include <gtest/gtest.h>
#include <sealib/choicedictionary.h>

using Sealib::ChoiceDictionary;

TEST(ChoiceDictionaryTest, choicedictionary_integrity) {
    unsigned long int size = 128;
    ChoiceDictionary c = ChoiceDictionary(size);

    // insert into Choice Dictionary and test if choice() returns the correct index
    c.insert(63UL);
    ASSERT_EQ(c.choice(), 63);
    c.insert(35UL);
    ASSERT_EQ(c.choice(), 35);
    c.insert(23UL);
    ASSERT_EQ(c.choice(), 23);
    c.insert(12UL);
    ASSERT_EQ(c.choice(), 12);
    c.insert(123UL);
    ASSERT_EQ(c.choice(), 123);
    c.insert(94UL);
    ASSERT_EQ(c.choice(), 94);


    // verify inserted values
    ASSERT_TRUE(c.get(63));
    ASSERT_TRUE(c.get(35));
    ASSERT_TRUE(c.get(23));
    ASSERT_TRUE(c.get(12));
    ASSERT_TRUE(c.get(123));
    ASSERT_TRUE(c.get(94));

    // verify empty values don't return false positives
    ASSERT_FALSE(c.get(1));
    ASSERT_FALSE(c.get(7));
    ASSERT_FALSE(c.get(8));
    ASSERT_FALSE(c.get(5));
}
