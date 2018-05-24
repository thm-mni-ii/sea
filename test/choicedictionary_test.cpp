#include <gtest/gtest.h>
#include <sealib/choicedictionary.h>

TEST(ChoiceDictionaryTest, choicedictionary_integrity)
{
    unsigned int size = 10;
    ChoiceDictionary c = ChoiceDictionary(size);

    // insert into Choice Dictionary and test if choice() returns the correct index
    c.insert(0);
    ASSERT_EQ(c.choice(), 0);
    c.insert(2);
    ASSERT_EQ(c.choice(), 2);
    c.insert(3);
    ASSERT_EQ(c.choice(), 3);
    c.insert(6);
    ASSERT_EQ(c.choice(), 6);
    c.insert(5);
    ASSERT_EQ(c.choice(), 5);
    c.insert(9);
    ASSERT_EQ(c.choice(), 9);

    // verify inserted values
    ASSERT_TRUE(c.get(0));
    ASSERT_TRUE(c.get(2));
    ASSERT_TRUE(c.get(3));
    ASSERT_TRUE(c.get(6));
    ASSERT_TRUE(c.get(5));
    ASSERT_TRUE(c.get(9));

    // verify empty values don't return false positives
    ASSERT_FALSE(c.get(1));
    ASSERT_FALSE(c.get(4));
    ASSERT_FALSE(c.get(7));
    ASSERT_FALSE(c.get(8));
}
